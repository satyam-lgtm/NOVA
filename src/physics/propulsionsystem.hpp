#pragma once
#include "../math/vec3.hpp"
#include "engine.hpp"
#include <memory>
#include <vector>

class PropulsionSystem {
private:
  std::vector<std::unique_ptr<Engine>> engines_;
  double totalFuelMass_;   // Current fuel mass (kg)
  double initialFuelMass_; // Initial fuel mass (kg)
  Vec3 thrustDirection_;   // Normalized thrust direction
  double gimbalAngleX_;    // Gimbal angle in X axis (rad)
  double gimbalAngleY_;    // Gimbal angle in Y axis (rad)
  double maxGimbalAngle_;  // Maximum gimbal angle (rad)

public:
  // Default constructor
  PropulsionSystem(double initialFuel, double maxGimbalAngleDeg = 5.0)
      : totalFuelMass_(initialFuel), initialFuelMass_(initialFuel),
        thrustDirection_(0, 0, 1) // Default thrust direction (up)
        ,
        gimbalAngleX_(0), gimbalAngleY_(0),
        maxGimbalAngle_(maxGimbalAngleDeg * M_PI / 180.0) {}

  // Delete copy constructor and assignment operator
  PropulsionSystem(const PropulsionSystem &) = delete;
  PropulsionSystem &operator=(const PropulsionSystem &) = delete;

  // Move constructor
  PropulsionSystem(PropulsionSystem &&other) noexcept
      : engines_(std::move(other.engines_)),
        totalFuelMass_(other.totalFuelMass_),
        initialFuelMass_(other.initialFuelMass_),
        thrustDirection_(other.thrustDirection_),
        gimbalAngleX_(other.gimbalAngleX_), gimbalAngleY_(other.gimbalAngleY_),
        maxGimbalAngle_(other.maxGimbalAngle_) {}

  // Move assignment operator
  PropulsionSystem &operator=(PropulsionSystem &&other) noexcept {
    if (this != &other) {
      engines_ = std::move(other.engines_);
      totalFuelMass_ = other.totalFuelMass_;
      initialFuelMass_ = other.initialFuelMass_;
      thrustDirection_ = other.thrustDirection_;
      gimbalAngleX_ = other.gimbalAngleX_;
      gimbalAngleY_ = other.gimbalAngleY_;
      maxGimbalAngle_ = other.maxGimbalAngle_;
    }
    return *this;
  }

  void addEngine(double maxThrust, double isp, double throatArea,
                 double expansionRatio) {
    engines_.push_back(
        std::make_unique<Engine>(maxThrust, isp, throatArea, expansionRatio));
  }

  Vec3 updateThrust(double atmosphericPressure, double dt) {
    if (totalFuelMass_ <= 0) {
      shutdownAllEngines();
      return Vec3();
    }

    double totalThrust = 0.0;
    double fuelConsumed = 0.0;

    for (auto &engine : engines_) {
      if (engine->isActive()) {
        totalThrust += engine->getCurrentThrust(atmosphericPressure);
        fuelConsumed += engine->getFuelConsumption(dt);
      }
    }

    totalFuelMass_ -= fuelConsumed;
    if (totalFuelMass_ < 0)
      totalFuelMass_ = 0;

    Vec3 gimbaled_direction = applyGimbal(thrustDirection_);
    return gimbaled_direction.normalize() * totalThrust;
  }
  void setGimbalAngles(double angleX, double angleY) {
    gimbalAngleX_ = std::clamp(angleX, -maxGimbalAngle_, maxGimbalAngle_);
    gimbalAngleY_ = std::clamp(angleY, -maxGimbalAngle_, maxGimbalAngle_);
  }

  double getRemainingFuelRatio() const {
    return totalFuelMass_ / initialFuelMass_;
  }

  void startEngines() {
    for (auto &engine : engines_) {
      engine->start();
    }
  }

  void shutdownAllEngines() {
    for (auto &engine : engines_) {
      engine->shutdown();
    }
  }

  void setThrottle(double throttle) {
    for (auto &engine : engines_) {
      engine->setThrottle(throttle);
    }
  }

  void updateThrustDirection(const Vec3 &position) {
    // Point thrust in the direction away from Earth's center
    thrustDirection_ = position.normalize();
  }

private:
  Vec3 applyGimbal(Vec3 &baseDirection) const {
    double cx = std::cos(gimbalAngleX_);
    double sx = std::sin(gimbalAngleX_);
    double cy = std::cos(gimbalAngleY_);
    double sy = std::sin(gimbalAngleY_);
    double x = baseDirection.x() * cy + baseDirection.z() * sy;
    double y = baseDirection.y() * cx -
               (baseDirection.x() * sy - baseDirection.z() * cy) * sx;
    double z = baseDirection.y() * sx +
               (baseDirection.x() * sy - baseDirection.z() * cy) * cx;
    return Vec3(x, y, z).normalize();
  }
};
