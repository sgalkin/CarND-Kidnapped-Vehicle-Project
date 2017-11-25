#pragma once

#include <string>
#include <vector>

#include "map.h"
#include "measurement.h"

struct WSProtocol {  
  static bool checkHeader(const std::string& message);

  static std::string getPayload(std::string message);
  static Measurement getMeasurement(std::string payload);
  
  static std::string formatResponse();
  static std::string formatResponse(const Position& best,
                                    const std::vector<Landmark>& associations);
};
