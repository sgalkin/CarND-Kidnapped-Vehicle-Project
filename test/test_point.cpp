#include <sstream>
#include <cmath>

#include "catch.hpp"
#include "../src/point.h"

TEST_CASE("IO") {
  std::istringstream iss("34.5 55.4");
  Point p;
  iss >> p;
  REQUIRE(Approx(34.5) == p.x);
  REQUIRE(Approx(55.4) == p.y);
}

TEST_CASE("Add") {
  const Point s{1,2};
  const Point d{3,4};
  SECTION("+") {
    Point x = s + d;
    REQUIRE(x.x == s.x + d.x);
    REQUIRE(x.y == s.y + d.y);
  }
  SECTION("+=") {
    Point x = s;
    x += d;
    REQUIRE(x.x == s.x + d.x);
    REQUIRE(x.y == s.y + d.y);
  }
}

TEST_CASE("Distance") {
  const Point s{-1, 0};
  const Point d{1, 3};
  SECTION("distance2") {
    REQUIRE(13 == distance2(s, d));
  }
  SECTION("distance") {
    REQUIRE(Approx(sqrt(13)) == distance(s, d));
  }
}

TEST_CASE("Probability") {
  Point sigma{0.3, 0.3};
  SECTION("obs1") {
    Point s{6, 3};
    Point l{5, 3};
    REQUIRE(Approx(0.00683644777551) == probability(s, l, sigma));
  }
  SECTION("obs2") {
    Point s{2, 2};
    Point l{2, 1}; 
    REQUIRE(Approx(0.00683644777551) == probability(s, l, sigma));
 }
  SECTION("obs3") {
    Point s{0, 5};
    Point l{2, 1};
    REQUIRE(Approx(9.83184874151e-49) == probability(s, l, sigma));
  }
}
