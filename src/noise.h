#pragma once

#include <random>
#include "point.h"
#include "measurement.h"

template<typename Engine>
Point noise(Point sigma, Engine& eng) {
  std::normal_distribution<decltype(sigma.x)> x{0, sigma.x};
  std::normal_distribution<decltype(sigma.y)> y{0, sigma.y};
  return Point{x(eng), y(eng)};
}

template<typename Engine>
Position noise(Position sigma, Engine& eng) {
  std::normal_distribution<decltype(sigma.theta)> theta{0, sigma.theta};
  return Position{noise(sigma.point, eng), theta(eng)};
}
