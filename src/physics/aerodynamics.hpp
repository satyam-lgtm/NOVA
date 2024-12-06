#pragma once
#include "aeroconstants.hpp"
#include "atmosphere.hpp"
#include "rocketbody.hpp"
#include "state.hpp"
#include <cmath>

class Aerodynamics {
public:
  static Vec3 calculateForces(const State &state, RocketBody &rocket,
                              const Vec3 &windVelocity = Vec3()) {
    Vec3 relativeVelocity = state.velocity - windVelocity;
    double velocityMagnitude = relativeVelocity.magnitude();

    if (velocityMagnitude < 1e-6)
      return Vec3();

    double altitude = state.position.magnitude() - Constants::EARTH_RADIUS;
    double airDensity = Atmosphere::getDensity(altitude);
    double temperature = Atmosphere::getTemperature(altitude);

    double soundSpeed = std::sqrt(
        AeroConstants::GAMMA * AeroConstants::AIR_GAS_CONSTANT * temperature);
    double machNumber = velocityMagnitude / soundSpeed;

    Vec3 verticalAxis(0, 0, 1);
    double angleOfAttack = std::acos(
        std::abs(relativeVelocity.dot(verticalAxis)) / velocityMagnitude);

    rocket.updateAeroCoefficients(machNumber, angleOfAttack);

    double dynamicPressure =
        0.5 * airDensity * velocityMagnitude * velocityMagnitude;

    Vec3 dragDirection = relativeVelocity.normalize() * -1.0;
    Vec3 dragForce =
        dragDirection * (dynamicPressure * rocket.getReferenceArea() *
                         rocket.getDragCoefficient());

    Vec3 liftDirection = relativeVelocity.cross(verticalAxis).normalize();
    Vec3 liftForce =
        liftDirection * (dynamicPressure * rocket.getReferenceArea() *
                         rocket.getLiftCoefficient());
    // Adding coriolis force vector
    Vec3 angularVelocityVec(0,0,-Constants::EARTH_ANGULAR_VELOCITY);
    Vec3 coriolisForce = angularVelocityVec.cross(state.velocity).operator*(-2.0 * rocket.getMass());

    return dragForce + liftForce + coriolisForce;
  }
  static double calculateDynamicPressure(const State &state) {
    double altitude = state.position.magnitude() - Constants::EARTH_RADIUS;
    double airDensity = Atmosphere::getDensity(altitude);
    double velMagnitude = state.velocity.magnitude();
    return 0.5 * airDensity * velMagnitude * velMagnitude;
  }
  static double calculateStagnationTemperature(const State &state,
                                               double ambientTemp) {
    double velocityMagnitude = state.velocity.magnitude();
    double machNumber =
        velocityMagnitude /
        std::sqrt(AeroConstants::GAMMA * AeroConstants::AIR_GAS_CONSTANT *
                  ambientTemp);

    return ambientTemp * (1.0 + ((AeroConstants::GAMMA - 1.0) / 2.0) *
                                    machNumber * machNumber);
  }
};
