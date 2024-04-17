#include "math/vector3.h"

/**
 * @brief Default constructor.
 */
Vector3::Vector3() {
  x = 0.0;
  y = 0.0;
  z = 0.0;
}
Vector3::Vector3(float x_, float y_, float z_) {
  x = x_;
  y = y_;
  z = z_;
}

Vector3 Vector3::operator+(const Vector3 &v) const {
  return {x + v.x, y + v.y, z + v.z};
}

Vector3 Vector3::operator-(const Vector3 &v) const {
  return {this->x - v.x, this->y - v.y, this->z - v.z};
}

Vector3 Vector3::operator*(float m) const {
  return {m * this->x, m * this->y, m * this->z};
}

Vector3 Vector3::operator/(float m) const {
  return {this->x / m, this->y / m, this->z / m};
}

void Vector3::Print() const {
  std::cout << "[" << this->x << ", " << this->y << ", " << this->z << "]"
            << std::endl;
}