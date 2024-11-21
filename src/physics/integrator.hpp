#pragma once
#include "state.hpp"
#include <functional>

class Integrator {
public:
  static State
  integrateRK4(const State &currentState,
               std::function<Vec3(const State &)> accelerationFunction,
               double dt) {
    Vec3 k1_a = accelerationFunction(currentState);
    Vec3 k1_v = currentState.velocity;

    State halfState1(currentState.position + k1_v * (dt / 2),
                     currentState.velocity + k1_a * (dt / 2), k1_a,
                     currentState.mass, currentState.time + dt / 2);

    Vec3 k2_a = accelerationFunction(halfState1);
    Vec3 k2_v = currentState.velocity + k1_a * (dt / 2);

    State halfState2(currentState.position + k2_v * (dt / 2),
                     currentState.velocity + k2_a * (dt / 2), k2_a,
                     currentState.mass, currentState.time + dt / 2);

    Vec3 k3_a = accelerationFunction(halfState2);
    Vec3 k3_v = currentState.velocity + k2_a * (dt / 2);

    State endState(currentState.position + k3_v * dt,
                   currentState.velocity + k3_a * dt, k3_a, currentState.mass,
                   currentState.time + dt);

    Vec3 k4_a = accelerationFunction(endState);
    Vec3 k4_v = currentState.velocity + k3_a * (dt / 2);

    Vec3 newVelocity = currentState.velocity +
                       (k1_a + k2_a * 2.0 + k3_a * 2.0 + k4_a) * (dt / 6.0);

    Vec3 newPosition = currentState.position +
                       (k1_v + k2_v * 2.0 + k3_v * 2.0 + k4_v) * (dt / 6.0);

    return State(
        newPosition, newVelocity,
        accelerationFunction(State(newPosition, newVelocity, Vec3(),
                                   currentState.mass, currentState.time + dt)),
        currentState.mass, currentState.time + dt);
  }
};
