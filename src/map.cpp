#include "map.h"
#include <algorithm>
#include <iterator>
#include <fstream>

namespace {
bool Xorder(const Map::Storage::value_type& lhs,
            const Map::Storage::value_type& rhs) {
  return lhs.position.x < rhs.position.x;
}

Map::Storage build_map(Map::Storage lm) {
  std::sort(begin(lm), end(lm), Xorder);
  return lm;
}

std::unordered_map<size_t, size_t> build_index(const Map::Storage& lm) {
  std::unordered_map<size_t, size_t> idx;
  for(size_t i = 0; i < lm.size(); ++i) {
    idx[lm[i].id] = i;
  }
  return idx;
}
}

Map::Map() {}
Map::Map(Storage lm) :
  map_(build_map(std::move(lm))),
  idx_(build_index(map_))
{}

const Landmark& Map::nearest(const Point& pnt, double range) const {
  static const Landmark not_found{INVALID_ID, {0, 0}};
    
  auto low = Landmark{INVALID_ID, {pnt.x - range, pnt.y}};
  auto high = Landmark{INVALID_ID, {pnt.x + range, pnt.y}};
    
  auto lb = std::lower_bound(begin(map_), end(map_), low, Xorder);
  auto ub = std::upper_bound(begin(map_), end(map_), high, Xorder);
  auto c = std::min_element(lb, ub,
                            [&pnt](const Storage::value_type& lhs,
                                   const Storage::value_type& rhs) {
                              return distance2(lhs.position, pnt) < distance2(rhs.position, pnt);
                            });
  return c == ub || distance2(c->position, pnt) > range*range ? not_found : *c;
}

Map read_map(const std::string& filename) {
  std::ifstream in(filename.c_str());
  if(!in) throw std::ios::failure("unable to open file");

  std::vector<Landmark> map;
  std::copy(std::istream_iterator<Landmark>(in),
            std::istream_iterator<Landmark>(),
            std::back_inserter(map));
  return Map{std::move(map)};
}
