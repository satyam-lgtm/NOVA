#pragma once
#include "../math/vec3.hpp"
#include <cmath>

class Engine {
private:
  double maxThrust_;
  double specificImpulse_;
  double throatArea_;
  double expansionRatio_;
  double massFlowRate_;
  double currentThrottle_;
  bool isActive_;

public:
  Engine(double maxThrust, double isp, double throatArea, double expansionRatio)
      : maxThrust_(maxThrust), specificImpulse_(isp), throatArea_(throatArea),
        expansionRatio_(expansionRatio),
        massFlowRate_(maxThrust / (specificImpulse_ * 9.81)),
        currentThrottle_(0.0), isActive_(false) {}

  void start() { isActive_ = true; }
  void shutdown() {
    isActive_ = false;
    currentThrottle_ = 0.0;
  }

  void setThrottle(double throttle) {
    currentThrottle_ = std::clamp(throttle, 0.0, 1.0);
  }
  double getCurrentThrust(double atmosphericPressure) const {
    if (!isActive_)
      return 0.0;

    double pressureRatio = atmosphericPressure / Constants::SEA_LEVEL_PRESSURE;
    double altitudeCompensation = 1.0 + (1.0 - pressureRatio) * 0.3;

    return maxThrust_ * currentThrottle_ * altitudeCompensation;
  }
  double getFuelConsumption(double dt) const {
    if (isActive_ == false)
      return 0.0;
    return massFlowRate_ * currentThrottle_ * dt;
  }

  bool isActive() const { return isActive_; }
  double getThrottle() const { return currentThrottle_; }
};
