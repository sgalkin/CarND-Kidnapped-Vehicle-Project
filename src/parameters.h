#pragma once

#include "point.h"
#include "measurement.h"

struct Parameters {
  // Number of particles in the filter
  static constexpr size_t particles_number{75};
  
  // GPS measurement uncertainty [x [m], y [m], theta [rad]]
  static const Position position_sigma;

  // Landmark measurement uncertainty [x [m], y [m]]
  static const Point landmark_sigma;

  // Sensor range [m]
  static constexpr double sensor_range{50};

  static const Interval dt;
};
