
#include "physics/aerodynamics.hpp"
#include "physics/atmosphere.hpp"
#include "physics/gravity.hpp"
#include "physics/integrator.hpp"
#include "physics/rocketbody.hpp"
#include "physics/state.hpp"
#include <iostream>

int main() {
  RocketBody rocket(20.0, 2.0, 5000.0, 2000.0);

  State state(Vec3(Constants::EARTH_RADIUS + 1000.0, 0, 0), Vec3(0, 100, 0),
              Vec3(), rocket.getMass(), 0.0);

  double dt = 0.01;
  double endTime = 30.0;

  while (state.time < endTime) {
    auto calculateAcceleration = [&rocket](const State &s) {
      Vec3 gravityForce = Gravity::getAcceleration(s.position) * s.mass;
      Vec3 aeroForce = Aerodynamics::calculateForces(s, rocket);
      return (gravityForce + aeroForce) / s.mass;
    };

    state = Integrator::integrateRK4(state, calculateAcceleration, dt);

    if (std::fmod(state.time, 1.0) < dt) {
      double altitude = state.position.magnitude() - Constants::EARTH_RADIUS;
      double velocity = state.velocity.magnitude();
      double dynamicPressure = Aerodynamics::calculateDynamicPressure(state);

      std::cout << "Time: " << state.time << "s\n"
                << "Altitude: " << altitude << " m\n"
                << "Velocity: " << velocity << " m/s\n"
                << "Acceleration: " << state.acceleration.magnitude()
                << "m/s^2\n"
                << "Dynamic Pressure: " << dynamicPressure << " Pa\n"
                << "Temperature: " << Atmosphere::getTemperature(altitude)
                << " K\n\n";
    }
  }

  return 0;
}
