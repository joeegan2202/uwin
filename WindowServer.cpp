//
// Created by joe on 2/26/24.
//

#include "WindowServer.hpp"

#include <chrono>
#include <thread>
#include <iostream>

#include <sys/mman.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

WindowServer::WindowServer(VideoDevice& device, const Image &image) : videoDevice(device), buffer(device.getWidth(), device.getHeight(), device.getChannels()), backgroundImage(image) {

}

WindowServer::~WindowServer() {

}

void WindowServer::start() {
	auto initial_time = std::chrono::high_resolution_clock::now();
  auto last_second = initial_time;

	int counter = 60*10;

  int frames = 0;

	//Temporarily count up to 10 seconds at 60 fps
	while(counter--) {
		buffer.blit(backgroundImage, 0, 0);
		// Render all 
		for(std::pair<int, Window *> p : activeWindows) {
			buffer.blit(p.second->framebuffer, p.second->x, p.second->y);
		}

		videoDevice.blit(buffer);
		frames++;

		auto now_time = std::chrono::high_resolution_clock::now();
		if(now_time - initial_time < std::chrono::duration<double,std::milli>{16.667}) {
			auto remaining = std::chrono::duration<double,std::milli>{16.667} - (now_time - initial_time);
			std::this_thread::sleep_for(remaining);
		} else {
			std::cout << "Skipped time in the frame!" << std::endl;
		}

		initial_time = std::chrono::high_resolution_clock::now();

    if(initial_time - last_second >= std::chrono::duration<double,std::milli>{1000}) {
      last_second = initial_time;
      std::cout << "FPS: " << frames << std::endl;
      frames = 0;
    }
	}
}

void WindowServer::newWindow(std::string name, int width, int height) {
	int mem_fd = memfd_create(name.c_str(), 0);

	int mem_size = width * height * 4;

	ftruncate(mem_fd, mem_size);

	int id = 0;
	while(activeWindows[id] == nullptr) id++;

	activeWindows[id] = new Window(name, mem_fd, width, height, 0, 0);

	WindowMessage msg = {
			.type = WINDOW_NEW,
			.body = { .newWindow = {
					.name = {0},
					.id = id,
			}}};
	memcpy(msg.body.newWindow.name, name.c_str(), name.length());

	sendMessageWithFileDescriptor(msg, mem_fd);
}

void WindowServer::sendMessageWithFileDescriptor(WindowMessage m, int f) {
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
			// Need nullptr because socket is already connected
			.msg_name = nullptr,
			.msg_namelen = 0,
			.msg_iov = &iov,
			.msg_iovlen = 1,
			.msg_control = c_header.buf,
			.msg_controllen = sizeof(c_header.buf),
			.msg_flags = 0
	};

	// get address of the first cmessage header we will write to
	cmsghdr *cmsgp;

	cmsgp = CMSG_FIRSTHDR(&header);
	cmsgp->cmsg_level = SOL_SOCKET;
	cmsgp->cmsg_type = SCM_RIGHTS;
	cmsgp->cmsg_len = CMSG_LEN(sizeof(int));
	memcpy(CMSG_DATA(cmsgp), &f, sizeof(int));

	sendmsg(sock_fd, &header, 0);
}
