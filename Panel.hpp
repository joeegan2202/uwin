#ifndef __PANEL_PANEL_HPP__
#define __PANEL_PANEL_HPP__

#include "Framebuffer.hpp"
#include "Window.hpp"

#include <unordered_map>

class Panel {
public:
  Panel();
  Framebuffer getPanel(const std::unordered_map<int, Window *> &windows);
  Image bar;
  Image buttonLeft;
  Image buttonRight;
  Image buttonFill;
};

#endif //__PANEL_PANEL_HPP__
