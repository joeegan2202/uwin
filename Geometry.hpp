#ifndef UWIN_GEOMETRY_HPP
#define UWIN_GEOMETRY_HPP

#include <utility>

struct Point {
	int x, y;
};

class Rectangle {
public:
	static Rectangle fromTwoPoints(Point a, Point c) {
		return Rectangle(a, Point{.x = a.x, .y = c.y}, c, Point{.x = c.x, .y = a.y});
	}
	static Rectangle fromWidthHeight(Point a, int w, int h) {
		return Rectangle(a, Point{.x = a.x + w, .y = a.y}, Point{.x = a.x + w, .y = a.y + h}, Point{.x = a.x, .y = a.y + h});
	}
private:
	Rectangle(Point a, Point b, Point c, Point d) : a(a), b(b), c(c), d(d) {}
	bool contains(Point p);
	Point a, b, c, d;
};

#endif