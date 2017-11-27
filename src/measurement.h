#pragma once

#include <iterator>
#include <vector>
#include <chrono>
#include "point.h"

// Struct representing one position/control measurement.
struct Control {
  double velocity;  // Velocity [m/s]
  double yawrate;	// Yaw rate [rad/s]
};

// Struct representing vehicle position
struct Position {
  Point point;   // Global vehicle position
  double theta;  // Global vehicle yaw [rad]
};

Position operator+ (Position lhs, Position rhs);

using Observation = std::vector<Point>;

Observation operator+ (Observation o, const Point& p);
Observation operator+ (const Point&p, Observation o);

using Measurement = std::tuple<Position, Control, Observation>;
using Interval = std::chrono::duration<double>;

// Control
Position move(Position p, Control c, const Interval& dt);

// Transformation
Point transform(Position pos, Point pnt);
Observation transform(Position pos, const Observation& o);

// IO
template<typename IS>
IS& operator>> (IS& is, Control& c) {
  is >> c.velocity >> c.yawrate;
  return is;
}

template<typename IS>
IS& operator>> (IS& is, Position& p) {
  is >> p.point >> p.theta;
  return is;
}

template<typename IS>
IS& operator>> (IS& is, Observation& p) {
  std::copy(std::istream_iterator<Point>(is),
            std::istream_iterator<Point>(),
            std::back_inserter(p));
  return is;
}
