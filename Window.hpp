#ifndef UWIN_WINDOW_HPP
#define UWIN_WINDOW_HPP

#include "Framebuffer.hpp"

#include <string>

class Window {
public:
	Window(std::string n, int mfd, int w, int h, int x, int y);
	~Window();
	Framebuffer framebuffer;
	int memFd;
	int width, height, x, y;
	std::string name;
	uint8_t *memory;
	int length;
};

#endif // UWIN_WINDOW_HPP
