#ifndef UWIN_MOUSE_HPP
#define UWIN_MOUSE_HPP

#include "Geometry.hpp"
#include <stack>
#include <atomic>

enum MouseMode {
	CURSOR,
	OTHER
};

void initializeMouse();
Point getLocation();
void setMode(MouseMode);
void pollEvents(std::atomic_bool&);

#endif