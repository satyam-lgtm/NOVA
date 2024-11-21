#pragma once
#include "../math/vec3.hpp"
#include <algorithm>

class RocketBody {
private:
  double length_;          // Length of rocket (m)
  double diameter_;        // Diameter of rocket (m)
  double referenceArea_;   // Reference area (m²)
  double dragCoefficient_; // Base drag coefficient
  double liftCoefficient_; // Base lift coefficient
  Vec3 centerOfMass_;      // Center of mass position
  double mass_;            // Current mass (kg)
  double wetMass_;         // Mass with full fuel (kg)
  double dryMass_;

public:
  RocketBody(double len, double dia, double wetM, double dryM)
      : length_(len), diameter_(dia), referenceArea_(3.14159 * dia * dia / 4.0),
        dragCoefficient_(0.0), liftCoefficient_(0.0), centerOfMass_(0, 0, 0),
        mass_(wetM), wetMass_(wetM), dryMass_(dryM) {}

  double getLength() const { return length_; }
  double getDiameter() const { return diameter_; }
  double getReferenceArea() const { return referenceArea_; }
  double getDragCoefficient() const { return dragCoefficient_; }
  double getLiftCoefficient() const { return liftCoefficient_; }
  double getMass() const { return mass_; }
  Vec3 getCenterOfMass() const { return centerOfMass_; }

  void updateMass(double fuelConsumed) {
    mass_ = std::max(wetMass_ - fuelConsumed, dryMass_);
  }

  void updateAeroCoefficients(double machNumber, double angleOfAttack) {
    if (machNumber < 0.8)
      dragCoefficient_ = 0.2;
    else if (machNumber < 1.2)
      dragCoefficient_ = 0.2 + 0.6 * (machNumber - 0.8);
    else
      dragCoefficient_ = 0.4;

    liftCoefficient_ = 0.1 * std::sin(2 * angleOfAttack);
  }
};
