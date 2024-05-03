//
// Created by joe on 2/26/24.
//

#include "WindowServer.hpp"

#include "EventManager.hpp"
#include "Mouse.hpp"
#include "Signal.hpp"
#include "Panel.hpp"
#include "LaunchHelper.hpp"
#include "libevdev/libevdev.h"

#include <chrono>
#include <functional>
#include <thread>
#include <list>
#include <iostream>
#include <tuple>
#include <climits>

#include <sys/mman.h>
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>

#define DEBUG(x) std::cout << std::chrono::high_resolution_clock::now().time_since_epoch().count() << " - " << x << std::endl;

std::stack<Signal> signals;

std::stack<Signal> listener_signals;

class CtrlAltDel : public EventListener {
  public:
    using EventListener::EventListener;
    CtrlAltDel() : EventListener(std::bind(&CtrlAltDel::ctrlAltDel, this, std::placeholders::_1)), ctrl(false), alt(false) {}
  private:
    bool ctrl, alt;

    void ctrlAltDel(std::shared_ptr<struct input_event> event) {
      if(event->type == EV_KEY) {
        if(event->code == KEY_LEFTCTRL) {
          ctrl = event->value;
        } else if(event->code == KEY_RIGHTCTRL) {
          ctrl = event->value;
        } else if(event->code == KEY_LEFTALT) {
          alt = event->value;
        } else if(event->code == KEY_RIGHTALT) {
          alt = event->value;
        } else if(event->code == KEY_BACKSPACE && ctrl && alt) {
          DEBUG("Attempting to quit...")
          signals.push({SIG_KILL, "CTRLALTDEL"});
        }
      }
    }
};

WindowServer::WindowServer(VideoDevice& device, const Image &image) : videoDevice(device), buffer(device.getWidth(), device.getHeight(), device.getChannels()), backgroundImage(image) {}

int max_orderedZ(std::unordered_map<int, WindowHolder> &map, int MAX) {
  int max = -1;
  for(auto [id, w] : map) {
    if(w.z_level > max && w.z_level < MAX) max = w.z_level;
  }

  return max;
}

int min_orderedZ(std::unordered_map<int, WindowHolder> &map) {
  int min = INT_MAX;
  for(auto [id, w] : map) {
    if(w.z_level < min) min = w.z_level;
  }

  return min;
}

void r_orderedZForeach(std::unordered_map<int, WindowHolder> &map, std::function<void(WindowHolder &)> func, int max) {
  func(map.at(max));

  if(max != min_orderedZ(map)) r_orderedZForeach(map, func, max_orderedZ(map, max));
}

void orderedZForeach(std::unordered_map<int, WindowHolder> &map, std::function<void(WindowHolder &)> func) {
  if(map.size() == 0) return;

  r_orderedZForeach(map, func, max_orderedZ(map, INT_MAX));
}

void WindowServer::start() {
	auto initial_time = std::chrono::high_resolution_clock::now();
  auto last_second = initial_time;

	int counter = 60*10;

  int frames = 0;

	should_run = true;
	std::thread listener(&WindowServer::listen, this);

  EventManager eMan;

  Mouse mouse(std::bind(&WindowServer::handleClickCallback, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&WindowServer::handleDragCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  mouse.setMode(MouseMode::CURSOR);

  eMan.registerListener(mouse.getListener());
  eMan.registerTimeKeeper(mouse.getTimeKeeper());

  /*
  auto printer = std::make_shared<EventListener>([](std::shared_ptr<struct input_event> event) {
      std::cout << "Event: " << libevdev_event_code_get_name(event->type, event->code) << " Type: " << event->type << " Code: " << event->code << " Value: " << event->value << std::endl;
  });
  eMan.registerListener(printer);
  */

  auto exit = std::make_shared<CtrlAltDel>();
  eMan.registerListener(exit);

	Image cursor("cursor.bmp");

  Panel panel;

  launchers.emplace_back("default_ico.bmp", "Default", "build/dumb-client", std::vector<std::string>{});

  //LauncherHelper::launch("build/dumb-client", {});

	while(should_run) {
    while(!signals.empty()) {
      auto signal = signals.top();

      std::cout << "Signal: \"" << signal.msg << "\"" << std::endl;
      if(signal.type == SIG_KILL) {
        should_run = false;
        listener_signals.push(signal);
      }

      signals.pop();
    }

    eMan.poll();

		buffer.blit(backgroundImage, 0, 0);

    Framebuffer panel_fb = panel.getPanel(liveWindows);
    buffer.blit(panel_fb, 0, 1080-panel_fb.getHeight());

    for(auto &launcher : launchers) {
      buffer.blit(launcher.getFramebuffer(), launcher.bounds.getTopLeft().x, launcher.bounds.getTopLeft().y);
    }

		// Render all
    {
      std::lock_guard<std::mutex> guard(liveWindows_m);
      orderedZForeach(liveWindows, [&buffer=buffer](WindowHolder &w) { 
          buffer.blit(w.t.fb, w.t.outer_bounds.getTopLeft().x, w.t.outer_bounds.getTopLeft().y);
          buffer.blit(w.w->framebuffer, w.w->bounds.getTopLeft().x, w.w->bounds.getTopLeft().y);
          });
		}

		Point loc = mouse.getLocation();
		buffer.blit(cursor, loc.x, loc.y);

		videoDevice.blit(buffer);
		frames++;

		auto now_time = std::chrono::high_resolution_clock::now();
		if(now_time - initial_time < std::chrono::duration<double,std::milli>{16.667}) {
			auto remaining = std::chrono::duration<double,std::milli>{16.667} - (now_time - initial_time);
			std::this_thread::sleep_for(remaining);
		} else {
			std::cout << "Skipped time in the frame!\n";
		}

		initial_time = std::chrono::high_resolution_clock::now();

    if(initial_time - last_second >= std::chrono::duration<double,std::milli>{1000}) {
      last_second = initial_time;
      std::cout << "FPS: " << frames << '\n';
      frames = 0;
    }
	}

  DEBUG("Waiting to join socket server thread...")
	listener.join();
	std::cout.flush();
}

void WindowServer::handleClickCallback(int button, Point location) {
  bool placed = false;
  {
    std::lock_guard<std::mutex> guard(liveWindows_m);

    orderedZForeach(liveWindows, [location,&placed](WindowHolder &w) {
      if(w.w->bounds.contains(location)) {
        // TODO: Assign event to window
        placed = true;
      }
    });
  }

  if(!placed) {
    for(auto &l : launchers) {
      if(l.bounds.contains(location)) {
        l.launch();
        placed = true;
        break;
      }
    }
  }

  /* For panel buttons... TODO
  if(!placed) {
    for(auto &l : launchers) {
      if(l.bounds.contains(location)) {
        l.launch();
        placed = true;
        break;
      }
    }
  }
  */
}

void WindowServer::handleDragCallback(int button, bool still_dragging, Point location) {
  std::cout << "window: " << dragged_window << ", still_dragging: " << still_dragging << ", {" << location.x << ", " << location.y << "}" << std::endl;
  if(dragged_window == 0) {
    {
      std::lock_guard<std::mutex> guard(liveWindows_m);

      std::cout << "Dragging when no dragged window!" << std::endl;
      orderedZForeach(liveWindows, [this,location](WindowHolder &w) {
        if(w.w->bounds.contains(location)) {
          // TODO: Assign event to window
          std::cout << "Dragging in window!" << std::endl;
        //} else if(w.t.outer_bounds.contains(location)) {
        } else {
          std::cout << "Caught panel!" << std::endl;
          dragged_window = w.w->id;
        }
      });
    }
  } else {
    {
      std::lock_guard<std::mutex> guard(liveWindows_m);
      std::cout << "Updating window!" << std::endl;
      auto &w = liveWindows.at(dragged_window);
      w.t.outer_bounds = Rectangle::fromWidthHeight({.x = w.t.outer_bounds.getTopLeft().x + location.x, .y = w.t.outer_bounds.getTopLeft().y + location.y}, w.t.outer_bounds.getWidth(), w.t.outer_bounds.getHeight());
      w.w->bounds = Rectangle::fromWidthHeight({.x = w.w->bounds.getTopLeft().x + location.x, .y = w.w->bounds.getTopLeft().y + location.y}, w.w->bounds.getWidth(), w.w->bounds.getHeight());
    }

    if(!still_dragging) dragged_window = 0;
  }
}

void WindowServer::newWindow(std::string name, int width, int height, struct sockaddr_un *addr, socklen_t *length) {
	DEBUG("Creating new window...")
	int mem_fd = memfd_create(name.c_str(), 0);

	int mem_size = width * height * 4;

	ftruncate(mem_fd, mem_size);

	int id = 0;
	{
		std::lock_guard<std::mutex> guard(liveWindows_m);
		while (liveWindows.find(id) != liveWindows.end())
			id++;

		Window *w = new Window(id, name, mem_fd, width, height, 100, 100);
    liveWindows.emplace(id, WindowHolder{w, Titlebar::make(w->name, w->bounds), id, id});
	}

	WindowMessage msg = {
			.type = WINDOW_NEW,
			.body = { .newWindow = {
					.name = {0},
					.id = id,
					.w = width,
					.h = height
			}}};
	memcpy(msg.body.newWindow.name, name.c_str(), name.length());

	DEBUG("Created window " + name)

	sendMessageWithFileDescriptor(msg, mem_fd, addr, length);
}

void WindowServer::sendMessageWithFileDescriptor(WindowMessage m, int f, struct sockaddr_un *addr, socklen_t *length) {
	DEBUG("Sending fd " << f << " to " << addr->sun_path)
	// IOVector to store window message
	iovec iov;
	// Stores cmsghdr in buffer that will be copied to a msghdr
	union
	{
		char buf[CMSG_SPACE(sizeof(int))];
		struct cmsghdr align;
	} c_header;

	// copy message to iov
	iov.iov_base = &m;
	iov.iov_len = sizeof(WindowMessage);

	msghdr header = {
			.msg_name = addr,
			.msg_namelen = *length,
			.msg_iov = &iov,
			.msg_iovlen = 1,
			.msg_control = c_header.buf,
			.msg_controllen = sizeof(c_header.buf),
			.msg_flags = 0,
	};

	// get address of the first cmessage header we will write to
	cmsghdr *cmsgp;

	cmsgp = CMSG_FIRSTHDR(&header);
	cmsgp->cmsg_level = SOL_SOCKET;
	cmsgp->cmsg_type = SCM_RIGHTS;
	cmsgp->cmsg_len = CMSG_LEN(sizeof(int));
	memcpy(CMSG_DATA(cmsgp), &f, sizeof(int));

	int return_val = sendmsg(sock_fd, &header, 0);

	DEBUG("Sent message with file descriptor!")
}

void WindowServer::listen() {
	sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);

	struct sockaddr_un addr = {0};
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "/run/user/1000/uwin-0");

	unlink(addr.sun_path);
	bind(sock_fd, (struct sockaddr *)&addr, sizeof(sockaddr_un));
	DEBUG("Bound server socket")

  struct pollfd pollfds[1] = {{sock_fd, POLLIN, 0}};
	WindowMessage request;
	struct sockaddr_un addr_from;
	socklen_t addr_length;
	while(should_run) {
    memset(&request, 0, sizeof(WindowMessage));
    memset(&addr_from, 0, sizeof(struct sockaddr_un));
    addr_length = sizeof(struct sockaddr_un);

    DEBUG("Server waiting for message")
    if(poll(pollfds, 1, 100) > 0) {
      int bytes_read = recvfrom(sock_fd, &request, sizeof(WindowMessage), 0, (struct sockaddr *)&addr_from, &addr_length);

      DEBUG("Got " << bytes_read << " bytes")

      if(bytes_read != sizeof(WindowMessage))
        std::cerr << "Read not enough bytes! Continuing..." << std::endl;

      switch(request.type) {
        case WINDOW_NEW:
          newWindow(request.body.newWindow.name, request.body.newWindow.w, request.body.newWindow.h, &addr_from, &addr_length);
          break;
        default:
          std::cerr << "Unknown window message received!" << std::endl;
      }
    }
	}
}
