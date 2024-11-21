#include "physics/atmosphere.hpp"
#include "physics/gravity.hpp"
#include "physics/integrator.hpp"
#include "physics/state.hpp"
#include <iostream>

int main() {

  State state(Vec3(Constants::EARTH_RADIUS + 1000.0, 0, 0), Vec3(0, 100, 0),
              Vec3(), 1000.0, 0.0);

  double dt = 0.01;
  double endTime = 10.0;

  while (state.time < endTime) {
    auto calculateAcceleration = [](const State &s) {
      return Gravity::getAcceleration(s.position);
    };

    state = Integrator::integrateRK4(state, calculateAcceleration, dt);

    if (std::fmod(state.time, 1.0) < dt) {
      std::cout << "Time: " << state.time << "s\n"
                << "Position (m): " << state.position.x() << ", "
                << state.position.y() << ", " << state.position.z() << "\n"
                << "Velocity (m/s): " << state.velocity.x() << ", "
                << state.velocity.y() << ", " << state.velocity.z() << "\n"
                << "Altitude (m): "
                << (state.position.magnitude() - Constants::EARTH_RADIUS)
                << "\n\n";
    }
  }
  return 0;
}
