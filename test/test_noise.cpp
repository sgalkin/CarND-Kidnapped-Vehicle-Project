#include "catch.hpp"

#include <random>
#include <algorithm>
#include "../src/noise.h"

TEST_CASE("Point Noise") {
  constexpr size_t N = 100000;
  Point sigma{1, 10};
  std::default_random_engine engine{42};
  std::vector<Point> n;
  std::generate_n(std::back_inserter(n), N,
                  [sigma, &engine]{ return noise(sigma, engine); });
  auto mmx = std::minmax_element(begin(n), end(n),
                                 [](const Point& lhs, const Point& rhs) {
                                   return lhs.x < rhs.x;
                                 });
  auto mmy = std::minmax_element(begin(n), end(n),
                                 [](const Point& lhs, const Point& rhs) {
                                   return lhs.y < rhs.y;
                                 });
  REQUIRE(std::get<1>(mmx)->x - std::get<0>(mmx)->x >= 6*sigma.x);
  REQUIRE(std::get<1>(mmy)->y - std::get<0>(mmy)->y >= 6*sigma.y);
}

TEST_CASE("Position Noise") {
  constexpr size_t N = 100000;
  Position sigma{{1, 100}, 0.1};
  std::default_random_engine engine{42};
  std::vector<Position> n;
  std::generate_n(std::back_inserter(n), N,
                  [sigma, &engine]{ return noise(sigma, engine); });
  auto mmx = std::minmax_element(begin(n), end(n),
                                 [](const Position& lhs, const Position& rhs) {
                                   return lhs.point.x < rhs.point.x;
                                 });
  auto mmy = std::minmax_element(begin(n), end(n),
                                 [](const Position& lhs, const Position& rhs) {
                                   return lhs.point.y < rhs.point.y;
                                 });
  auto mmt = std::minmax_element(begin(n), end(n),
                                 [](const Position& lhs, const Position& rhs) {
                                   return lhs.theta < rhs.theta;
                                 });
  REQUIRE(std::get<1>(mmx)->point.x - std::get<0>(mmx)->point.x >= 6*sigma.point.x);
  REQUIRE(std::get<1>(mmy)->point.y - std::get<0>(mmy)->point.y >= 6*sigma.point.y);
  REQUIRE(std::get<1>(mmt)->theta - std::get<0>(mmt)->theta >= 6*sigma.theta);
}
