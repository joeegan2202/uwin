#include "EventManager.hpp"
#include "libevdev/libevdev.h"

#include <stdexcept>
#include <iostream>

#include <fcntl.h>

EventListener::EventListener(std::function<void(std::shared_ptr<struct input_event>)> callback) : callback(callback) {}

void EventListener::run(std::shared_ptr<struct input_event> event) {
  callback(event);
}

EventManager::EventManager() {
  for(int i = 0; i < 100; i++) {
    int fd = open(("/dev/input/event" + std::to_string(i)).c_str(), O_RDONLY);
    if(fd < 0)
      continue;

    std::cout << "Adding listener to /dev/input/event" << i << std::endl;

    auto new_device = libevdev_new();
    if(!new_device)
      throw std::runtime_error("libevdev_new");
    devices.push_back(new_device);

    if(libevdev_set_fd(new_device, fd) < 0) {
      std::cerr << "Error setting fd on /dev/input/event" << i << std::endl;
      throw std::runtime_error("libevdev_set_fd");
    }
  }
}

EventManager::~EventManager() {
  for(auto device : devices)
    libevdev_free(device);
}

void EventManager::registerListener(std::weak_ptr<EventListener> listener) {
  listeners.insert(listener);
}

void EventManager::registerTimeKeeper(std::weak_ptr<TimeKeeper> time_keeper) {
  time_keepers.insert(time_keeper);
}

void EventManager::poll() {
  bool inputs = true;
  while(inputs) {
    inputs = false;
    for(auto device : devices) {
      while(libevdev_has_event_pending(device)) {
        inputs = true;
        auto event = std::make_shared<struct input_event>();

        libevdev_next_event(device, LIBEVDEV_READ_FLAG_NORMAL, event.get());

        if(event->type == EV_SYN) {
          if(event->code == SYN_REPORT) break;

          switch(event->code) {
            case SYN_DROPPED:
              std::cerr << "TODO: Dropped events in poll event" << std::endl;
              break;
            default:
              std::cerr << "Unexpected SYN_... code polling events" << std::endl;
              break;
          }
        } else {
          dispatch(event);
        }
      }
    }
  }

  keepTime();
}

void EventManager::dispatch(std::shared_ptr<struct input_event> event) {
  for(auto i = listeners.begin(); i != listeners.end();) {
    auto listener = i->lock();
    if(listener) {
      listener->run(event);
      i++;
    } else {
      i = listeners.erase(i);
    }
  };
}

void EventManager::keepTime() {
  for(auto i = time_keepers.begin(); i != time_keepers.end();) {
    auto time_keeper = i->lock();
    if(time_keeper) {
      time_keeper->run(std::chrono::high_resolution_clock::now());
      i++;
    } else {
      i = time_keepers.erase(i);
    }
  }
}

TimeKeeper::TimeKeeper(std::function<void(std::chrono::high_resolution_clock::time_point)> callback) : callback(callback) {}

void TimeKeeper::run(std::chrono::high_resolution_clock::time_point time) {
  callback(time);
}
