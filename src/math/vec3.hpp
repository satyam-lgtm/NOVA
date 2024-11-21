#pragma once
#include <cmath>

class Vec3 {
private:
  double x_, y_, z_;

public:
  Vec3() : x_(0.0), y_(0.0), z_(0.0) {}
  Vec3(double x, double y, double z) : x_(x), y_(y), z_(z) {}

  double x() { return x_; }
  double y() { return y_; }
  double z() { return z_; }

  Vec3 operator+(const Vec3 &other) const {
    return Vec3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
  }
  Vec3 operator-(const Vec3 &other) const {
    return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
  }
  Vec3 operator*(double scalar) const {
    return Vec3(x_ * scalar, y_ * scalar, z_ * scalar);
  }
  Vec3 operator/(double scalar) const {
    return Vec3(x_ / scalar, y_ / scalar, z_ / scalar);
  }
  double dot(const Vec3 &other) const {
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
  }
  Vec3 cross(const Vec3 &other) const {
    return Vec3(y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_,
                x_ * other.y_ - y_ * other.x_);
  }

  double magnitude() const { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
  Vec3 normalize() const {
    double m = magnitude();
    if (m <= 0.0) {
      return *this;
    }
    return *this / m;
  }
};
