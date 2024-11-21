#include "../visualization/display.hpp"
#include "physics/simulationengine.hpp"
#include <fstream>
#include <iostream>

int main() {
  try {
    RocketBody rocket(20.0,    // Length (m)
                      2.0,     // Diameter (m)
                      5000.0,  // Wet mass (kg)
                      2000.0); // Dry mass (kg)

    PropulsionSystem propulsion(3000.0);              // 3000 kg of fuel
    propulsion.addEngine(100000.0, 300.0, 0.5, 20.0); // Add main engine

    State initialState(Vec3(Constants::EARTH_RADIUS + 100.0, 0,
                            0),          // 100m above Earth's surface
                       Vec3(0, 0, 0),    // Initial velocity
                       Vec3(),           // Initial acceleration
                       rocket.getMass(), // Initial mass
                       0.0               // Initial time
    );

    SimulationEngine sim(initialState, rocket, std::move(propulsion));
    Display display;

    sim.startEngines();
    sim.setThrottle(1.0);

    std::ofstream dataFile("flight_data.csv");
    dataFile << "Time,Altitude,Velocity,Acceleration,Mass,Fuel_Ratio\n";

    while (!display.shouldClose()) {
      const State &state = sim.getState();

      // Update simulation based on UI controls
      sim.setThrottle(display.getThrottle());
      sim.setGimbalAngles(display.getGimbalX(), display.getGimbalY());

      // Update visualization
      display.render(state, rocket, sim.getTime());

      // Log data
      if (std::fmod(sim.getTime(), 0.1) < 0.01) {
        double altitude = state.position.magnitude() - Constants::EARTH_RADIUS;
        double velocity = state.velocity.magnitude();
        double acceleration = state.acceleration.magnitude();

        dataFile << sim.getTime() << "," << altitude << "," << velocity << ","
                 << acceleration << "," << state.mass << ","
                 << sim.getRemainingFuelRatio() << "\n";
      }

      sim.step();
    }

    dataFile.close();
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
