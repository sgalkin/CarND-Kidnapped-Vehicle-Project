#include <sys/stat.h>

#include "catch.hpp"
#include "../src/map.h"

TEST_CASE("Map Data") {
  SECTION("Happy path") {
    auto m = read_map("../data/map_data.txt");
    REQUIRE(42 == m.size());
    REQUIRE(Approx(69.2) == m[10].position.x);
    REQUIRE(Approx(-78.217) == m[10].position.y);
    REQUIRE(10 == m[10].id);
  }
  
  SECTION("Not exists") {
    struct stat buffer;
    constexpr char name[] = "fake.file.name";
    REQUIRE(-1 == stat(name, &buffer));
    REQUIRE_THROWS_AS(read_map(name), std::ios::failure);
  }
}

TEST_CASE("Nearest neighbour") {
  constexpr double range = 2;
  Map m{std::vector<Landmark>{{1, {0, 1}}, {2, {1, 1}}}};
  SECTION("Out of the range x") {
    Point p{-2, 0};
    REQUIRE(Map::INVALID_ID == m.nearest(p, range).id);
  }
  SECTION("Out of the range y") {
    Point p{1, 4};
    REQUIRE(Map::INVALID_ID == m.nearest(p, range).id);
  }
  SECTION("In range") {
    Point p{0, 0};
    REQUIRE(1 == m.nearest(p, range).id);
  }
}
