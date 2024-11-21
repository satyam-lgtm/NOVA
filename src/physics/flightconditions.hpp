#pragma once
#include "../math/vec3.hpp"

struct FlightConditions {
  double machNumber;
  double angleOfAttack;
  double dynamicPressure;
  double reynoldsNumber;
  Vec3 windVelocity;

  FlightConditions()
      : machNumber(0.0), angleOfAttack(0.0), dynamicPressure(0.0),
        reynoldsNumber(0.0), windVelocity() {}
};
