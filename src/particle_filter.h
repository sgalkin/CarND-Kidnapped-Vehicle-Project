#pragma once

#include <vector>
#include "measurement.h"
#include "noise.h"
#include "map.h"

class Particle {
public:
  using Associations = std::vector<Landmark>;
  
  Particle();
  explicit Particle(Position p, double w = 1.0);

  void predict(const Control& c, const Interval& dt, Position noise);
  void update(const Observation& noisy, Point sigma, const Map& map, double range);

  double weight() const { return weight_; }
  Position position() const { return position_; }
  const Associations& associations() const { return associations_; }
  
private:
  void associate(Observation o, const Map& map, double range);
  void weight(const Map& map, Point sigma);

private:
  double weight_;
  Position position_;
  Associations associations_;
};


template<typename Parameters, typename Engine>
class Filter {
  using State = std::vector<Particle>;
  
public:
  Filter(const Position& p, const Map& map, Engine engine) :
    map_{map},
    engine_{std::move(engine)} {
    state_.reserve(Parameters::particles_number);
    std::generate_n(std::back_inserter(state_), Parameters::particles_number,
                    [this, &p]{
                      return Particle{p + noise(Parameters::position_sigma, engine_)};
                    });
  }
  
  void feed(Control c, Interval dt, Observation o) {
    std::vector<double> w;
    w.reserve(state_.size());
    std::for_each(begin(state_), end(state_),
                  [this, &c, &dt, &o, &w](State::value_type& p) {
                    p.predict(c, dt, noise(Parameters::position_sigma, engine_));
                    p.update(o, Parameters::landmark_sigma, map_, Parameters::sensor_range);
                    w.push_back(p.weight());
                  });
    resample(std::move(w));
  }

  const State::value_type& estimate() const {
    return *std::max_element(begin(state_), end(state_),
                             [](const typename State::value_type& lhs,
                                const typename State::value_type& rhs) {
                               return lhs.weight() < rhs.weight();
                             });
  }

private:
  void resample(std::vector<double> w) {
    std::discrete_distribution<size_t> idx{begin(w), end(w)};
    State state;
    state.reserve(state_.size());
    std::generate_n(std::back_inserter(state), state_.size(),
                    [this, &idx]() {
                      return state_[idx(engine_)];
                    });
    state_.swap(state);
  }
  
private:
  State state_;
  const Map& map_;
  Engine engine_;
};
