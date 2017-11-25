#include "measurement.h"
#include <algorithm>
#include <cmath>

Position operator+ (Position lhs, Position rhs) {
  return Position{lhs.point + rhs.point, lhs.theta + rhs.theta};
}

Position move(Position p, Control c, const Interval& duration) {
  auto dt = duration.count();
  auto dtheta = c.yawrate * dt / 2;
  auto limit = dtheta == 0 ? 1 : sin(dtheta) / (dtheta); // limit x->0 sin(x)/x == 1
  auto dangle = p.theta + dtheta;
  return (p +
          Position{{
              c.velocity*dt*cos(dangle)*limit,
              c.velocity*dt*sin(dangle)*limit
            },
            2*dtheta
  });
}

Point transform(Position pos, Point pnt) {
  return (pos.point +
          Point{
            pnt.x*cos(pos.theta) - pnt.y*sin(pos.theta),
            pnt.x*sin(pos.theta) + pnt.y*cos(pos.theta)
         });
}

Observation transform(Position pos, const Observation& o) {
  Observation r;
  r.reserve(o.size());
  std::transform(begin(o), end(o), std::back_inserter(r),
                 [&pos](const Observation::value_type& v) {
                   return transform(pos, v);
                 });
  return r;
}
