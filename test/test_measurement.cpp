#include "catch.hpp"

#include <cmath>
#include <limits>
#include "../src/measurement.h"

TEST_CASE("Move") {
  SECTION("yawrate == 0") {
    constexpr Control c{1, 0};
    constexpr Position p{{1, -1}, M_PI/2};

    auto m = move(p, c, Interval{1});

    REQUIRE(Approx(1).margin(1e-16) == m.point.x);
    REQUIRE(Approx(0).margin(1e-16) == m.point.y);
    REQUIRE(Approx(M_PI/2).margin(1e-16) == m.theta);
  } 
  SECTION("yawrate is small") {
    constexpr Control c{1, std::numeric_limits<double>::epsilon()};
    constexpr Position p{{0, 0}, M_PI/2};

    auto m = move(p, c, Interval{1});

    REQUIRE(Approx(0).margin(1e-8) == m.point.x);
    REQUIRE(Approx(1).margin(1e-8) == m.point.y);
    REQUIRE(Approx(M_PI/2).margin(1e-8) == m.theta);
  }  
  SECTION("yawrate != 0") {
    constexpr Control c{1, M_PI/4};
    constexpr Position p{{0, 0}, M_PI/4};

    auto m = move(p, c, Interval{1});
    
    REQUIRE(Approx(4/M_PI*(1-sqrt(2)/2)) == m.point.x);
    REQUIRE(Approx(4/M_PI*sqrt(2)/2) == m.point.y);
    REQUIRE(Approx(M_PI/2) == m.theta);
  }
}

TEST_CASE("Transform") {
  Position p{{4, 5}, -M_PI/2};
  SECTION("obs 1") {
    Point o{2, 2};
    auto r = transform(p, o);
    REQUIRE(Approx(6) == r.x);
    REQUIRE(Approx(3) == r.y);
  }
  SECTION("obs 2") {
    Point o{3, -2};
    auto r = transform(p, o);
    REQUIRE(Approx(2) == r.x);
    REQUIRE(Approx(2) == r.y);
  }
  SECTION("obs 3") {
    Point o{0, -4};
    auto r = transform(p, o);
    REQUIRE(Approx(0) == r.x);
    REQUIRE(Approx(5) == r.y);    
  }
}
