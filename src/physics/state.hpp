#pragma once
#include "../math/vec3.hpp"

struct State {
  Vec3 position, velocity, acceleration;
  double mass, time;

  State() : position(), velocity(), acceleration(), mass(0.0), time(0.0) {}
  State(const Vec3 &pos, const Vec3 &vel, const Vec3 &acc, const double &m,
        const double &t)
      : position(pos), velocity(vel), acceleration(acc), mass(m), time(t) {}
};
