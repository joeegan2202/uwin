#include "Mouse.hpp"

#include <unistd.h>
#include <iostream>

Mouse::Mouse() : listener(std::make_shared<EventListener>(std::bind(&Mouse::listen, this, std::placeholders::_1))), currentLocation{100, 100} {

}

Mouse::~Mouse() {

}

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

std::weak_ptr<EventListener> Mouse::getListener() {
  return listener;
}
