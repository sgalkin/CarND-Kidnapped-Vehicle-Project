#pragma once

#include <string>
#include <exception>
#include <memory>

#include "map.h"
#include "measurement.h"
#include "particle_filter.h"

template<typename Protocol, typename Parameters, typename Engine>
class Application {
  using Filter = Filter<Parameters, Engine>;

public:
  explicit Application(const Map& map, Engine e) :
    engine_{std::move(e)},
    map_{map}
  {}

  std::string ProcessMessage(std::string message) {
    if (!Protocol::checkHeader(message)) {
      throw std::runtime_error("Unexpected message header");  
    }
    return ProcessPayload(Protocol::getPayload(std::move(message)));
  }

private:
  std::string ProcessPayload(std::string payload) {
    if(payload.empty()) {
      return Protocol::formatResponse();
    }
    return ProcessMeasurement(Protocol::getMeasurement(std::move(payload)));
  }
  
  std::string ProcessMeasurement(Measurement measurement) {
    const auto& p = std::get<0>(measurement);
    const auto& c = std::get<1>(measurement);
    const auto& o = std::get<2>(measurement);
    
    if(!filter_) {
      filter_.reset(new Filter(p, map_, engine_));
    } else {
      filter_->feed(std::move(c), Parameters::dt, std::move(o));
    }
    const auto& b = filter_->estimate();
    return Protocol::formatResponse(b.position(), b.associations());
  }

private:
  Engine engine_;
  Map map_;
  std::unique_ptr<Filter> filter_;
};
