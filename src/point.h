#pragma once

// Struct representing one 2d point.
struct Point {
  double x;  // x position [m]
  double y;  // y position [m]
};

Point operator+ (Point lhs, Point rhs);
Point& operator+= (Point& lhs, Point rhs);
bool operator== (const Point& lhs, const Point& rhs);

double distance(Point lhs, Point rhs);
double distance2(Point lhs, Point rhs);

double probability(Point v, Point m, Point sigma);

template<typename IS>
IS& operator>> (IS& is, Point& p) {
  is >> p.x >> p.y;
  return is;
}

template<typename OS>
OS& operator<< (OS& os, const Point& p) {
  return os << p.x << " " << p.y;
}
