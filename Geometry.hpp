#ifndef UWIN_GEOMETRY_HPP
#define UWIN_GEOMETRY_HPP

struct Point {
	int x, y;
};

/*
 * a   b
 * d   c
 */
class Rectangle {
public:
	static Rectangle fromTwoPoints(Point a, Point c);
	static Rectangle fromWidthHeight(Point a, int w, int h);
	bool contains(Point p);
  int getWidth();
  int getHeight();
  Point getTopLeft();
  Point getTopRight();
  Point getBottomRight();
  Point getBottomLeft();
private:
	Rectangle(Point a, Point b, Point c, Point d);
	Point a, b, c, d;
};

#endif
