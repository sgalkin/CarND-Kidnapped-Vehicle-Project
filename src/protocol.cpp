#include "protocol.h"

#include <type_traits>
#include <iterator>

#include "json.hpp"
#include "measurement.h"

namespace {
template<typename C, typename Op>
std::string dump(const C& c, Op op) {
  std::stringstream s;
  std::transform(begin(c), end(c),
                 std::ostream_iterator<typename std::result_of<Op(Landmark)>::type>(s, " "),
                 op);
  std::string r = s.str();
  return r.empty() ? r : r.substr(0, r.size() - 1);
}
}

// for convenience
using json = nlohmann::json;

bool WSProtocol::checkHeader(const std::string& message) {
  // "42" at the start of the message means there's a websocket message event.
  // The 4 signifies a websocket message. The 2 signifies a websocket event
  return message.length() > 2 && strncmp(message.data(), "42", 2) == 0;
}

std::string WSProtocol::getPayload(std::string s) {
  // Checks if the SocketIO event has JSON data.
  // If there is data the JSON object in string format will be returned,
  // else the empty string "" will be returned.
  auto b1 = s.find_first_of("["), b2 = s.find_last_of("]");
  if (s.find("null") == std::string::npos &&
      b1 != std::string::npos &&
      b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

Measurement WSProtocol::getMeasurement(std::string s) {
  auto j = json::parse(s);
  if (j[0].get<std::string>() != "telemetry") {
    throw std::runtime_error("Unexpected event type");
  }
  auto& data = j[1];
  Position p{{std::stod(data["sense_x"].get<std::string>()),
              std::stod(data["sense_y"].get<std::string>())},
             std::stod(data["sense_theta"].get<std::string>())};

  Control c{std::stod(data["previous_velocity"].get<std::string>()),
            std::stod(data["previous_yawrate"].get<std::string>())};
  
  Observation o;
  std::istringstream xss{data["sense_observations_x"].get<std::string>()};
  std::istringstream yss{data["sense_observations_y"].get<std::string>()};
  std::transform(std::istream_iterator<double>{xss},
                 std::istream_iterator<double>{},
                 std::istream_iterator<double>{yss},
                 std::back_inserter(o),
                 [](double x, double y) { return Point{x, y}; });
  return Measurement{std::move(p), std::move(c), std::move(o)};
}
  
std::string WSProtocol::formatResponse() {
  static const std::string manual = "42[\"manual\",{}]";
  return manual;
}

std::string WSProtocol::formatResponse(const Position& best,
                                       const std::vector<Landmark>& associations) {
  json msgJson;

  msgJson["best_particle_x"] = best.point.x;
  msgJson["best_particle_y"] = best.point.y;
  msgJson["best_particle_theta"] = best.theta;

  //Optional message data used for debugging particle's sensing and associations
  msgJson["best_particle_associations"] = dump(associations,
                                               [](const Landmark& l) { return l.id; });
  msgJson["best_particle_sense_x"] = dump(associations,
                                          [](const Landmark& l) { return l.position.x; });
  msgJson["best_particle_sense_y"] = dump(associations,
                                          [](const Landmark& l) { return l.position.y; });

  return "42[\"best_particle\"," + msgJson.dump() + "]";
}
