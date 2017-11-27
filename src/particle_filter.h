#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include "measurement.h"
#include "noise.h"
#include "map.h"

class Particle {
public:
  using Associations = std::vector<Landmark>;
  
  Particle();
  explicit Particle(Position p, double w = 1.0);

  /**
   * predict Predicts the state for the next time step using the process model.
   * @param c - Control data 
   *    - Velocity of car from t to t+1 [m/s]
   *    - Yaw rate of car from t to t+1 [rad/s]
   * @param dt Time between time step t and t+1 in measurements [s]
   * @param noise Noise to add to predicted position 
   *    - x [m]
   *    - y [m]
   *    - yaw [rad]
   */
  void predict(const Control& c, const Interval& dt, Position noise);

  /**
   * update Updates the weight for particle based on the likelihood of the observed measurements.
   * @param noisy Vector of landmark observations
   * @param sigma Landmark measurement uncertainty [x [m], y [m]]]
   * @param map Map class containing map landmarks
   * @param range Range [m] of sensor
   */
  void update(const Observation& noisy, Point sigma, const Map& map, double range);

  /**
   * Accessors
   */
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

/**
 * Particle Filter class
 * @param Parameters - class of global parameters, contains
 *   number of particles in the filter
 *   standard deviation of postion - x, y, and yaw
 *   standard deviation of landmark - x, and y
 * @param Engine - random engine to use
 */
template<typename Parameters, typename Engine>
class Filter {
  using State = std::vector<Particle>;
  
public:
  /**
   * constructor = init, RAII is a great C++ idiom, one should prefe RAII instead of 2 step 
   * intializatio where pissuble
   * Initializes particle filter by initializing particles to Gaussian
   *   distribution around first position and all the weights to 1.
   * @param p simulated estimate from GPS (contains x [m], y [m], and theta [rad])
   */
  Filter(const Position& p, const Map& map, Engine engine) :
    // TODO:
    // - Set the number of particles.
    // - Initialize all particles to first position
    //   (based on estimates of x, y, theta and their uncertainties from GPS) and all weights to 1.
    // - Add random Gaussian noise to each particle.
    map_{map},
    engine_{std::move(engine)} {
    state_.reserve(Parameters::particles_number);
    // create particles_number particles
    std::generate_n(std::back_inserter(state_), Parameters::particles_number,
                    [this, &p]{
                      // initialize each particle by given position + noise - N(0, sigma)
                      return Particle{p + noise(Parameters::position_sigma, engine_)};
                    });
  }

  /**
   * feed One iteration of the filter 
   * - calls predict-update cycle for each particle
   * - resamples the population
   * @param c - control data - velocity [m/s], and yawrate [rad/s]
   * @param dt - time interval
   * @param o - list of observations
   */
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

  /**
   * estimate Returns the best - one with the highest weight - particle of the filter
   */
  const State::value_type& estimate() const {
    return *std::max_element(begin(state_), end(state_),
                             [](const typename State::value_type& lhs,
                                const typename State::value_type& rhs) {
                               return lhs.weight() < rhs.weight();
                             });
  }

private:
  /**
   * resample Resamples from the updated set of particles to form the new set of particles.
   */
  void resample(std::vector<double> w) {
    // TODO: Resample particles with replacement with probability proportional to their weight.
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
