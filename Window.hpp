#ifndef UWIN_WINDOW_HPP
#define UWIN_WINDOW_HPP

#include "Framebuffer.hpp"
#include "Geometry.hpp"

#include <string>

class Window {
public:
	Window(int id, std::string n, int mfd, int w, int h, int x, int y);
	~Window();
	Framebuffer framebuffer;
  int id;
	int memFd;
  Rectangle bounds;
	std::string name;
	uint8_t *memory;
	int length;
};

#endif // UWIN_WINDOW_HPP
