//
// Created by joe on 2/28/24.
//

#include "WindowClient.hpp"

#include <iostream> 
#include <chrono>

#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>
#include <string.h>

#define DEBUG(x) std::cout << x << std::endl;

using namespace uwin;

WindowClient::WindowClient() {

}

WindowClient::~WindowClient() {

}

void WindowClient::establish() {
	sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path = {0}
	};
	strcpy(addr.sun_path, "/run/user/1000/uwin-0");

	pid_t pid = getpid();
	struct sockaddr_un my_addr = {
		.sun_family = AF_UNIX,
		.sun_path = {0}
	};
	memcpy(&(my_addr.sun_path[1]), &pid, sizeof(pid_t));

	DEBUG("Attempting to bind...")

	int res = bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_un));

	DEBUG("Bound " << res)

	WindowMessage new_window_message = {
		.type = WINDOW_NEW,
		.body = {
			.newWindow = {
				.id = 0,
				.w = 544,
				.h = 420
			}
		}
	};
	strcpy(new_window_message.body.newWindow.name, "Test Window");

	sendto(sock_fd, &new_window_message, sizeof(WindowMessage), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));

	DEBUG("Sent message")

	auto timeout = std::chrono::high_resolution_clock::now();
	WindowMessage response;
	ssize_t nr;
	struct iovec iov;
	struct msghdr msgh;

	/* Allocate a char buffer for the ancillary data. See the comments
		 in sendfd() */
	union
	{
		char buf[CMSG_SPACE(sizeof(WindowMessage))];
		struct cmsghdr align;
	} controlMsg;
	struct cmsghdr *cmsgp;

	/* The 'msg_name' field can be used to obtain the address of the
		 sending socket. However, we do not need this information. */

	msgh.msg_name = NULL;
	msgh.msg_namelen = 0;

	/* Specify buffer for receiving real data */

	msgh.msg_iov = &iov;
	msgh.msg_iovlen = 1;
	iov.iov_base = &response; /* Real data is an 'int' */
	iov.iov_len = sizeof(WindowMessage);

	/* Set 'msghdr' fields that describe ancillary data */

	msgh.msg_control = controlMsg.buf;
	msgh.msg_controllen = sizeof(controlMsg.buf);

	/* Receive real plus ancillary data; real data is ignored */

	struct pollfd fds = {
		.fd = sock_fd,
		.events = POLLIN,
		.revents = 0
	};
	int ret = poll(&fds, 1, 500); // 500ms timeout

	if(ret == 0) {
		std::cerr << "Timed out!!" << std::endl;
		return;
	}

	nr = recvmsg(sock_fd, &msgh, 0);
	if (nr == -1)
		std::cerr << "Problem!" << std::endl;

	cmsgp = CMSG_FIRSTHDR(&msgh);

	/* Check the validity of the 'cmsghdr' */

	if (cmsgp == NULL || cmsgp->cmsg_len != CMSG_LEN(sizeof(int)) || cmsgp->cmsg_level != SOL_SOCKET || cmsgp->cmsg_type != SCM_RIGHTS)
	{
		errno = EINVAL;
		std::cerr << "Problem!" << std::endl;
	}

	/* Return the received file descriptor to our caller */

	int fbuf_fd;
	memcpy(&fbuf_fd, CMSG_DATA(cmsgp), sizeof(int));

	DEBUG("Got fd " << fbuf_fd)

	fbuf = new Framebuffer(fbuf_fd, response.body.newWindow.w, response.body.newWindow.h, 4);
}
