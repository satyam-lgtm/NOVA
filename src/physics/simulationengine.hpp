#pragma once
#include "aerodynamics.hpp"
#include "gravity.hpp"
#include "integrator.hpp"
#include "propulsionsystem.hpp"
#include "rocketbody.hpp"
#include "state.hpp"
#include <iostream>

class SimulationEngine {
private:
  State state_;
  RocketBody rocket_;
  PropulsionSystem propulsion_;
  double timeStep_;
  double totalTime_;

public:
  // Modified constructor to take PropulsionSystem by rvalue reference
  SimulationEngine(const State &initialState, const RocketBody &rocket,
                   PropulsionSystem &&propulsion, double dt = 0.01)
      : state_(initialState), rocket_(rocket),
        propulsion_(std::move(propulsion)) // Use std::move here
        ,
        timeStep_(dt), totalTime_(0.0) {}

  // Delete copy constructor and assignment operator
  SimulationEngine(const SimulationEngine &) = delete;
  SimulationEngine &operator=(const SimulationEngine &) = delete;

  // Add move constructor and assignment operator
  SimulationEngine(SimulationEngine &&other) noexcept
      : state_(std::move(other.state_)), rocket_(std::move(other.rocket_)),
        propulsion_(std::move(other.propulsion_)), timeStep_(other.timeStep_),
        totalTime_(other.totalTime_) {}

  SimulationEngine &operator=(SimulationEngine &&other) noexcept {
    if (this != &other) {
      state_ = std::move(other.state_);
      rocket_ = std::move(other.rocket_);
      propulsion_ = std::move(other.propulsion_);
      timeStep_ = other.timeStep_;
      totalTime_ = other.totalTime_;
    }
    return *this;
  }

  void step() {
    double altitude = state_.position.magnitude() - Constants::EARTH_RADIUS;
    double pressure = Atmosphere::getPressure(altitude);

    // Update thrust direction to point away from Earth
    propulsion_.updateThrustDirection(state_.position);

    auto calculateAcceleration = [this, pressure](const State &s) {
      // Calculate gravitational force (points towards Earth's center)
      Vec3 gravityForce = Gravity::getAcceleration(s.position) * s.mass;

      // Calculate aerodynamic forces
      Vec3 aeroForce = Aerodynamics::calculateForces(s, rocket_);

      // Get thrust force (should point away from Earth)
      Vec3 thrustForce = propulsion_.updateThrust(pressure, timeStep_);

      // Debug output
      if (std::fmod(totalTime_, 1.0) < timeStep_) {
        std::cout << "Forces (N):"
                  << "\nGravity: " << gravityForce.magnitude()
                  << "\nThrust: " << thrustForce.magnitude()
                  << "\nAero: " << aeroForce.magnitude() << std::endl;
      }

      // Calculate total acceleration
      Vec3 totalForce = gravityForce + aeroForce + thrustForce;
      return totalForce / s.mass;
    };

    // Update state using RK4 integration
    state_ = Integrator::integrateRK4(state_, calculateAcceleration, timeStep_);
    totalTime_ += timeStep_;

    // Update rocket mass based on remaining fuel
    double fuelRatio = propulsion_.getRemainingFuelRatio();
    rocket_.updateMass(fuelRatio);
    state_.mass = rocket_.getMass();

    // Update center of mass and aerodynamic properties
    rocket_.updateCenterOfMass();
    double velocity = state_.velocity.magnitude();
    if (velocity > 0) {
      double machNumber = velocity / 340.0; // Approximate speed of sound
      double angleOfAttack = 0.0; // We could calculate this properly if needed
      rocket_.updateAeroCoefficients(machNumber, angleOfAttack);
    }
  }
  void startEngines() { propulsion_.startEngines(); }
  void setThrottle(double throttle) { propulsion_.setThrottle(throttle); }
  const State &getState() const { return state_; }
  double getTime() const { return totalTime_; }
  double getRemainingFuelRatio() const {
    return propulsion_.getRemainingFuelRatio();
  }
};
