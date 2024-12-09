#include "physics/simulationengine.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "../libs/json.hpp"

using json = nlohmann::json;

void parseConfig(const std::string& fileToOpen, RocketBody& rocket, PropulsionSystem& prop){
  std::ifstream file(fileToOpen);
  
  json config;
  file >> config;

  double length = config["rocket"]["length"];
  double diameter = config["rocket"]["diameter"];
  double wetMass = config["rocket"]["wet_mass"];
  double dryMass = config["rocket"]["dry_mass"];
  double fuelMass = config["propulsion"]["fuel_mass"];
  //support for multiple engines
  const auto& engines = config["propulsion"]["engines"];

  rocket = RocketBody(length,diameter,wetMass,dryMass);
  prop = PropulsionSystem(fuelMass);

  for(const auto& engine : engines){
    double thrust = engine["thrust"];
    double burn_rate = engine["burn_rate"];
    double efficiency = engine["efficiency"];
    double nozzleDiameter = engine["nozzle_diameter"];
    prop.addEngine(thrust, burn_rate, efficiency, nozzleDiameter);
  }
}

int main() {
  try {

    RocketBody rocket(0,0,2,1); // to not trigger error of dry mass > wet mass
    PropulsionSystem propulsion(0);
    parseConfig("src/config.json", rocket, propulsion);

    // Set initial state (100m above Earth's surface)
    State initialState(Vec3(Constants::EARTH_RADIUS + 100.0, 0, 0), // Position
                       Vec3(0, 0, 0),    // Initial velocity
                       Vec3(),           // Initial acceleration
                       rocket.getMass(), // Initial mass
                       0.0);             // Initial time

    // Initialize simulation
    SimulationEngine sim(initialState, rocket, std::move(propulsion));

    // Start engines at full throttle
    sim.startEngines();
    sim.setThrottle(1.0);

    // Create and setup CSV file
    std::ofstream dataFile("flight_data.csv");
    dataFile << std::fixed << std::setprecision(6);
    dataFile
        << "Time,Altitude,Velocity_X,Velocity_Y,Velocity_Z,Velocity_Magnitude,"
        << "Acceleration_X,Acceleration_Y,Acceleration_Z,Acceleration_"
           "Magnitude,"
        << "Mass,Fuel_Ratio,Air_Density,Air_Pressure,Temperature,"
        << "Dynamic_Pressure,Mach_Number,Drag_Coefficient,Lift_Coefficient\n";

    // Run simulation for 10 seconds
    double endTime = 100.0;
    while (sim.getTime() <= endTime) {
      const State &state = sim.getState();

      // Calculate current conditions
      double altitude = state.position.magnitude() - Constants::EARTH_RADIUS;
      double velocity_mag = state.velocity.magnitude();
      double accel_mag = state.acceleration.magnitude();
      double air_density = Atmosphere::getDensity(altitude);
      double air_pressure = Atmosphere::getPressure(altitude);
      double temperature = Atmosphere::getTemperature(altitude);
      double dynamic_pressure = Aerodynamics::calculateDynamicPressure(state);
      double mach_number =
          velocity_mag /
          std::sqrt(AeroConstants::GAMMA * AeroConstants::AIR_GAS_CONSTANT *
                    temperature);

      // Log data every second
      if (std::fmod(sim.getTime(), 1.0) < 0.01) {
        dataFile << sim.getTime() << "," << altitude << ","
                 << state.velocity.x() << "," << state.velocity.y() << ","
                 << state.velocity.z() << "," << velocity_mag << ","
                 << state.acceleration.x() << "," << state.acceleration.y()
                 << "," << state.acceleration.z() << "," << accel_mag << ","
                 << state.mass << "," << sim.getRemainingFuelRatio() << ","
                 << air_density << "," << air_pressure << "," << temperature
                 << "," << dynamic_pressure << "," << mach_number << ","
                 << rocket.getDragCoefficient() << ","
                 << rocket.getLiftCoefficient() << "\n";

        // Print progress to console
        std::cout << "Time: " << std::setprecision(1) << std::fixed
                  << sim.getTime() << "s, Altitude: " << std::setprecision(1)
                  << altitude << "m, Velocity: " << velocity_mag << "m/s\n";
      }

      // Check if the altitude is below zero
      if (altitude < 0) {
        std::cout << "The rocket has crashed.\n";
        break; // Exit the simulation loop
      }

      sim.step();
    }

    dataFile.close();
    std::cout << "\nSimulation completed. Data saved to flight_data.csv\n";
    return 0;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
