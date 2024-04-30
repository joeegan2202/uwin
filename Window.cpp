#include "Window.hpp"

#include <unistd.h>
#include <sys/mman.h>

Window::Window(std::string n, int mfd, int w, int h, int x, int y) : name(n), memFd(mfd), bounds(Rectangle::fromWidthHeight({.x = x, .y = y}, w, h)), framebuffer(Framebuffer(mfd, w, h, 4)) {}

Window::~Window() {
	munmap(memory, length);
	close(memFd);
}
