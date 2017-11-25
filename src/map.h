#pragma once

#include <string>
#include <vector>
#include <limits>
#include <unordered_map>

#include "point.h"

// Struct representing one landmark observation measurement.
struct Landmark {
  int id;          // Id of matching landmark in the map.
  Point position;  // Position of landmark
};

class Map {
public:
  using Storage = std::vector<Landmark>;
  enum { INVALID_ID = std::numeric_limits<decltype(Landmark::id)>::min() }; // TODO: optional

public:
  Map();
  explicit Map(Storage lm);

  size_t size() const { return map_.size(); }
  
  const Landmark& operator[] (size_t id) const { return map_[idx_.at(id)]; }
  const Landmark& nearest(const Point& pnt, double range) const;
  
private:
  Storage map_;
  std::unordered_map<size_t, size_t> idx_;
};

Map read_map(const std::string& filename);

template<typename IS>
IS& operator>> (IS& is, Landmark& lm) {
  is >> lm.position >> lm.id;
  return is;
}
