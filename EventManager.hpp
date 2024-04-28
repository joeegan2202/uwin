#ifndef __EVENT_MANAGER_HPP__
#define __EVENT_MANAGER_HPP__

#include <memory>
#include <functional>
#include <set>

#include <libevdev/libevdev.h>

class EventListener {
  public:
    EventListener(std::function<void(std::shared_ptr<struct input_event>)> callback);
    ~EventListener();
    void run(std::shared_ptr<struct input_event> event);
  private:
    std::function<void(std::shared_ptr<struct input_event>)> callback;
};

class EventManager {
  public:
    EventManager();
    ~EventManager();
    void registerListener(std::weak_ptr<EventListener> listener);
    void poll();
  private:
    // Evdev stuff
    std::vector<struct libevdev*> devices;
    // My attrs
    std::set<std::weak_ptr<EventListener>, std::owner_less<std::weak_ptr<EventListener>>> listeners;
    void dispatch(std::shared_ptr<struct input_event> event);
};

#endif // __EVENT_MANAGER_HPP__
