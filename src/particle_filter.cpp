#include "particle_filter.h"

Particle::Particle() :
  weight_{0}, position_{{0, 0}, 0}
{}
  
Particle::Particle(Position p, double w) :
  weight_{w}, position_{std::move(p)}
{}

void Particle::predict(const Control& c, const Interval& dt, Position noise) {
  position_ = move(std::move(position_), c, dt) + std::move(noise);
}

void Particle::update(const Observation& noisy, Point sigma, const Map& map, double range) {
  associate(transform(position_, noisy), map, range);
  weight(map, std::move(sigma));
}

void Particle::associate(Observation o, const Map& map, double range) {
  Associations().swap(associations_);
  associations_.reserve(o.size());
  std::transform(begin(o), end(o), std::back_inserter(associations_),
                 [&map, range](const Observation::value_type& v) {
                   return Associations::value_type{map.nearest(v, range).id, v};
                 });
}

void Particle::weight(const Map& map, Point sigma) {
  weight_ = std::accumulate(begin(associations_), end(associations_), 1.,
                            [&map, sigma](double w, const Associations::value_type& v) {
                              if(w == 0 || v.id == Map::INVALID_ID) return 0.;
                              return w * probability(v.position, map[v.id].position, sigma);
                            });
}
