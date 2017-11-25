#include "catch.hpp"
/*
#include <random>
#include <cmath>

#include "../src/particle_filter.h"

struct TestParameters {
  static const Position position_sigma;
};

const Position TestParameters::position_sigma{{1, 2}, 0.1};

TEST_CASE("Particle Filter") {
  static constexpr size_t N = 10000;
  using Filter = Filter<N, TestParameters>;
  static constexpr auto in_3s = [](double v, double m, double s) {
    return v >= m - 3*s && v <= m + 3*s;
  };
  static constexpr double TH = 0.996;

  constexpr Position p{{100, 200}, M_PI/2};

  std::default_random_engine eng{42};

  SECTION("Init") {
    auto state{Filter::init(p, eng)};
    size_t dx = std::count_if(begin(state), end(state),
                              [&p](const Particle& v) {
                                return in_3s(v.position.point.x,
                                             p.point.x,
                                             TestParameters::position_sigma.point.x);
                              });
    size_t dy = std::count_if(begin(state), end(state),
                              [&p](const Particle& v) {
                                return in_3s(v.position.point.y,
                                             p.point.y,
                                             TestParameters::position_sigma.point.y);
                              });
    size_t dth = std::count_if(begin(state), end(state),
                               [&p](const Particle& v) {
                                 return in_3s(v.position.theta,
                                              p.theta,
                                              TestParameters::position_sigma.theta);
                               });
    REQUIRE(N == state.size());
    REQUIRE(TH < double(dx)/N);
    REQUIRE(TH < double(dy)/N);
    REQUIRE(TH < double(dth)/N);
    REQUIRE(std::all_of(begin(state), end(state),
                        [](const Particle& v) { return Approx(1.0) == v.weight; }));
    REQUIRE(std::all_of(begin(state), end(state),
                        [](const Particle& v) { return v.associations.empty(); }));
  }
*/
  /*
  SECTION("Move") {
    auto state{Filter::init(p, p_s, eng)};
    state = Filter::predict(std::move(state), Control{1, 0}, Interval{0.1}, p_s, eng);
  }
  SECTION("Resample") {
    auto state{Filter::init(p, p_s, eng)};
    state = Filter::resample(std::move(state), eng);
  }
  SECTION("Update") {
    auto state{Filter::init(p, p_s, eng)};
    state = Filter::update(std::move(state), {}, {0.3, 0.3}, {}, 50);
  }
  */
//}

