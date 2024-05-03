#ifndef __TITLEBAR_TITLEBAR_HPP__
#define __TITLEBAR_TITLEBAR_HPP__

#include "Framebuffer.hpp"
#include "Geometry.hpp"
#include <string>

class Titlebar {
  static Image topLeft, topRight, topFill, leftFill, rightFill, bottomFill, bottomLeft, bottomRight;
  Titlebar(Framebuffer fb, Rectangle outer_bounds);
  public:
  Framebuffer fb;
  Rectangle outer_bounds;
  static void ensureImages();
  static Titlebar make(std::string name, Rectangle inner_bounds);
};

#endif //__TITLEBAR_TITLEBAR_HPP__
