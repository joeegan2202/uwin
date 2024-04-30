#ifndef UWIN_MOUSE_HPP
#define UWIN_MOUSE_HPP

#include "EventManager.hpp"
#include "Geometry.hpp"

#include <chrono>
#include <list>
#include <functional>

enum MouseMode {
	CURSOR,
	RELATIVE
};

struct Pending {
  int button;
  Point position;
  std::chrono::high_resolution_clock::time_point time;
  bool down;
};

class Mouse {
  public:
    Mouse(std::function<void(int,Point)> click_callback, std::function<void(int,bool,Point)> drag_callback);
    Point getLocation();
    void setMode(MouseMode);
    std::weak_ptr<EventListener> getListener();
    std::weak_ptr<TimeKeeper> getTimeKeeper();
  private:
    std::function<void(int,Point)> click_callback;
    std::function<void(int,bool,Point)> drag_callback;
    std::shared_ptr<EventListener> listener;
    std::shared_ptr<TimeKeeper> time_keeper;
    MouseMode mode;
    Point currentLocation;

    std::list<Pending> pending_m_events;

    void listen(std::shared_ptr<struct input_event>);
    void keepTime(std::chrono::high_resolution_clock::time_point);
};

#endif
