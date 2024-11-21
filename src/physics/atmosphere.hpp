#pragma once
#include "constants.hpp"
#include <cmath>

class Atmosphere {
public:
  static double getDensity(double altitude) {
    const double scaleHeight = 7400.0;
    return 1.225 * std::exp(-altitude / scaleHeight);
  }
  static double getPressure(double altitude) {
    const double scaleHeight = 7400;
    return Constants::SEA_LEVEL_PRESSURE * std::exp(-altitude / scaleHeight);
  }
  static double getTemperature(double altitude) {
    const double lapseRate = -0.0065;
    return Constants::SEA_LEVEL_TEMPERATURE + lapseRate * altitude;
  }
};
