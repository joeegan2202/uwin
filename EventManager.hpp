#ifndef __EVENT_MANAGER_HPP__
#define __EVENT_MANAGER_HPP__

#include <chrono>
#include <memory>
#include <functional>
#include <set>

#include <libevdev/libevdev.h>

class EventListener {
  public:
    EventListener(std::function<void(std::shared_ptr<struct input_event>)> callback);
    void run(std::shared_ptr<struct input_event> event);
  private:
    std::function<void(std::shared_ptr<struct input_event>)> callback;
};

class TimeKeeper {
  public:
    TimeKeeper(std::function<void(std::chrono::high_resolution_clock::time_point)> callback);
    void run(std::chrono::high_resolution_clock::time_point time);
  private:
    std::function<void(std::chrono::high_resolution_clock::time_point)> callback;
};

class EventManager {
  public:
    EventManager();
    EventManager(const EventManager &) = delete;
    EventManager(EventManager &&) = delete;
    ~EventManager();
    void registerListener(std::weak_ptr<EventListener> listener);
    void registerTimeKeeper(std::weak_ptr<TimeKeeper> time_keeper);
    void poll();
  private:
    // Evdev stuff
    std::vector<struct libevdev*> devices;
    // My attrs
    std::set<std::weak_ptr<EventListener>, std::owner_less<std::weak_ptr<EventListener>>> listeners;
    std::set<std::weak_ptr<TimeKeeper>, std::owner_less<std::weak_ptr<TimeKeeper>>> time_keepers;
    void keepTime();
    void dispatch(std::shared_ptr<struct input_event> event);
};

#endif // __EVENT_MANAGER_HPP__
