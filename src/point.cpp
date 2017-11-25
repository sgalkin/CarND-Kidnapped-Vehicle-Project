#include "point.h"

#include <cmath>

Point operator+ (Point lhs, Point rhs) {
  Point tmp{lhs};
  return tmp += rhs;
}

Point& operator+= (Point& lhs, Point rhs) {
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}

bool operator== (const Point& lhs, const Point& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

double distance(Point lhs, Point rhs) {
  return sqrt(distance2(std::move(lhs), std::move(rhs)));
}

double distance2(Point lhs, Point rhs) {
  auto dx = lhs.x - rhs.x;
  auto dy = lhs.y - rhs.y;
  return dx*dx + dy*dy;
}

double probability(Point v, Point m, Point sigma) {
  auto sx = 2*sigma.x*sigma.x;
  auto sy = 2*sigma.y*sigma.y;
  auto sxy = 2*sigma.x*sigma.y;
  auto dx = (m.x - v.x);
  auto dy = (m.y - v.y);
  auto e = dx*dx/sx + dy*dy/sy;
  return exp(-e)/(M_PI*sxy);
}
