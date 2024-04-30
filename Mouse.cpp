#include "Mouse.hpp"

#include <chrono>
#include <unistd.h>
#include <iostream>

Mouse::Mouse(std::function<void(int,Point)> click_callback, std::function<void(int,bool,Point)> drag_callback) :
  listener(std::make_shared<EventListener>(std::bind(&Mouse::listen, this, std::placeholders::_1))),
  time_keeper(std::make_shared<TimeKeeper>(std::bind(&Mouse::keepTime, this, std::placeholders::_1))),
  click_callback(click_callback),
  drag_callback(drag_callback),
  currentLocation{100, 100} {}

Point Mouse::getLocation() {
  return currentLocation;
}

void Mouse::setMode(MouseMode m) {
  mode = m;
}

void Mouse::listen(std::shared_ptr<struct input_event> event) {
  switch(event->type) {
    case EV_REL:
      if(event->code == REL_X) {
        currentLocation.x += event->value;
        if(currentLocation.x < 0) currentLocation.x = 0;
        if(currentLocation.x > 1919) currentLocation.x = 1919;
      } else if(event->code == REL_Y) {
        currentLocation.y += event->value;
        if(currentLocation.y < 0) currentLocation.y = 0;
        if(currentLocation.y > 1079) currentLocation.y = 1079;
      }
      break;
    case EV_KEY:
      if(mode == MouseMode::CURSOR) {
        if(event->value == 0) {
          // Button up
          for(auto &pending : pending_m_events) {
            if(pending.button == event->code) {
              if(!pending.down) std::cerr << "Funky timing error where a pending button is up, and also sees up event!" << std::endl;
              pending.down = false;
            }
          }
        } else if(event->value == 1) {
          // Button down
          if(event->code == BTN_LEFT || event->code == BTN_RIGHT || event->code == BTN_MIDDLE) {
            pending_m_events.emplace_back(Pending{.button = event->code, .position = currentLocation, .time = std::chrono::high_resolution_clock::now(), .down = true});
          }
        } else {
          std::cerr << "Unknown mouse button value!" << std::endl;
        }
      } else {
        if(event->code == BTN_LEFT) {
          std::cout << "Left button" << std::endl;
        } else if(event->code == BTN_RIGHT) {
          std::cout << "Right button" << std::endl;
        } else if(event->code == BTN_MIDDLE) {
          std::cout << "Middle button" << std::endl;
        }
        break;
      }
  }
}

void Mouse::keepTime(std::chrono::high_resolution_clock::time_point now) {
  for(auto i = pending_m_events.begin(); i != pending_m_events.end();) {
    auto &pending = *i;

    if(pending.down) {
      if(std::abs(pending.position.x - currentLocation.x) > 2 || std::abs(pending.position.y - currentLocation.y) > 2) {
        // We're handling an in-progress drag, not a click
        drag_callback(pending.button, true, Point{.x = currentLocation.x - pending.position.x, .y = currentLocation.y - pending.position.y});
        // Update location and time
        pending.position = currentLocation;
        pending.time = now;
      }

      i++;
    } else {
      if(std::abs(pending.position.x - currentLocation.x) > 2 || std::abs(pending.position.y - currentLocation.y) > 2) {
        // We're handling a drag, not a click
        drag_callback(pending.button, false, Point{.x = currentLocation.x - pending.position.x, .y = currentLocation.y - pending.position.y});
      } else {
        // Simple click, recognized by listen()
        click_callback(pending.button, currentLocation);
      }

      i = pending_m_events.erase(i);
    }
  }
}

std::weak_ptr<EventListener> Mouse::getListener() {
  return listener;
}

std::weak_ptr<TimeKeeper> Mouse::getTimeKeeper() {
  return time_keeper;
}
