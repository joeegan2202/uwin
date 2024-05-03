#include "Geometry.hpp"

Rectangle::Rectangle(Point a, Point b, Point c, Point d) : a(a), b(b), c(c), d(d) {}

Rectangle Rectangle::fromTwoPoints(Point a, Point c) {
	return Rectangle(a, Point{.x = c.x, .y = a.y}, c, Point{.x = a.x, .y = c.y});
}

Rectangle Rectangle::fromWidthHeight(Point a, int w, int h) {
	return Rectangle(a, Point{.x = a.x + w, .y = a.y}, Point{.x = a.x + w, .y = a.y + h}, Point{.x = a.x, .y = a.y + h});
}

bool Rectangle::contains(Point p) {
  return a.x <= p.x && d.x <= p.x &&
    b.x >= p.x && c.x >= p.x &&
    a.y <= p.y && b.y <= p.y &&
    d.y >= p.y && c.y >= p.y;
}

int Rectangle::getWidth() {
  return b.x - a.x;
}

int Rectangle::getHeight() {
  return d.y - a.y;
}

Point Rectangle::getTopLeft() {
  return a;
}

Point Rectangle::getTopRight() {
  return b;
}

Point Rectangle::getBottomRight() {
  return c;
}

Point Rectangle::getBottomLeft() {
  return d;
}
