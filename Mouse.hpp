#ifndef UWIN_MOUSE_HPP
#define UWIN_MOUSE_HPP

#include "EventManager.hpp"
#include "Geometry.hpp"

enum MouseMode {
	CURSOR,
	RELATIVE
};

class Mouse {
  public:
    Mouse();
    ~Mouse();
    Point getLocation();
    void setMode(MouseMode);
    std::weak_ptr<EventListener> getListener();
  private:
    std::shared_ptr<EventListener> listener;
    MouseMode mode;
    Point currentLocation;

    void listen(std::shared_ptr<struct input_event>);
};

#endif
