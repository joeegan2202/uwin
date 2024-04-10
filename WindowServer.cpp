//
// Created by joe on 2/26/24.
//

#include "WindowServer.hpp"

#include "Mouse.hpp"
#include "Cursor.hpp"

#include <chrono>
#include <thread>
#include <iostream>

#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define DEBUG(x) std::cout << x << std::endl;

WindowServer::WindowServer(VideoDevice& device, const Image &image) : videoDevice(device), buffer(device.getWidth(), device.getHeight(), device.getChannels()), backgroundImage(image) {

}

WindowServer::~WindowServer() {

}

void WindowServer::start() {
	auto initial_time = std::chrono::high_resolution_clock::now();
  auto last_second = initial_time;

	int counter = 60*10;

  int frames = 0;

	should_run = true;
	std::thread listener(&WindowServer::listen, this);

	initializeMouse();
	std::thread mouse_input(pollEvents, std::ref(should_run));

	Cursor cursor;

	pid_t id = fork();
	if(id == 0) {
		execl("build/dumb-client", (char *)NULL);
		std::cerr << "Didn't exec!!!" << std::endl;
		return;
	}

	//Temporarily count up to 10 seconds at 60 fps
	while(counter--) {
		buffer.blit(backgroundImage, 0, 0);
		// Render all
		{
			std::lock_guard<std::mutex> guard(activeWindows_m);
			for (std::pair<int, Window *> p : activeWindows)
			{
				buffer.blit(p.second->framebuffer, p.second->x, p.second->y);
			}
		}

		Point loc = getLocation();
		buffer.blit(cursor.icon, loc.x, loc.y);

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

	should_run = false;
	listener.join();

	std::cout.flush();
}

void WindowServer::newWindow(std::string name, int width, int height, struct sockaddr_un *addr, socklen_t *length) {
	DEBUG("Creating new window...")
	int mem_fd = memfd_create(name.c_str(), 0);

	int mem_size = width * height * 4;

	ftruncate(mem_fd, mem_size);

	int id = 0;
	{
		std::lock_guard<std::mutex> guard(activeWindows_m);
		while (activeWindows.find(id) != activeWindows.end())
			id++;

		activeWindows[id] = new Window(name, mem_fd, width, height, 0, 0);
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

	WindowMessage request;
	struct sockaddr_un addr_from;
	socklen_t addr_length;
	while(should_run) {
		memset(&request, 0, sizeof(WindowMessage));
		memset(&addr_from, 0, sizeof(struct sockaddr_un));
		addr_length = sizeof(struct sockaddr_un);

		DEBUG("Server waiting for message")
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
