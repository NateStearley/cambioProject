#ifndef Vector3_H_
#define Vector3_H_

#include <cmath>
#include <iostream>
#include <vector>

/// @brief A class built solely to handle 3D vector mathematics.
class Vector3 {
 public:
  float x = 0;  //!< x-coordinate
  float y = 0;  //!< y-coordinate
  float z = 0;  //!< z-coordinate

  /**
   * @brief Default constructor.
   */
  Vector3();

  /**
   * @brief Parameter constructor.
   *
   * @param[in] x_ x-coordinate
   * @param[in] y_ y-coordinate
   * @param[in] z_ z-coordinate
   */
  Vector3(float x_, float y_, float z_);

  /**
   * @brief Overrides + operator.
   * @param[in] v The Vector3 object you would like to add to this Vector3
   * object
   * @return The Vector3 Object comprised of the sum of the two objects
   */
  Vector3 operator+(const Vector3 &v) const;

  /**
   * @brief Overrides - operator.
   * @param[in] v The Vector3 object you would like to subtract to this Vector3
   * object
   * @return The Vector3 Object comprised of the subtraction of the two objects
   */

  Vector3 operator-(const Vector3 &v) const;
  /**
   * @brief Overrides * operator.
   * @param[in] m The Vector3 object you would like to multiply to this Vector3
   * object
   * @return The Vector3 Object comprised of the multiplication of the two
   * objects
   */
  Vector3 operator*(float m) const;

  /**
   * @brief Overrides / operator.
   * @param[in] m The Vector3 object you would like to divide to this Vector3
   * object
   * @return The Vector3 Object comprised of the division of the two objects
   */
  Vector3 operator/(float m) const;

  /**
   * @brief Overrides &[] operator.
   * @param[in] index The Vector3 object you want to get data 
   * from
   * @return The Vector3 x, y , or z index
   */
  float &operator[](int index) {
    if (index == 0) {
      return x;
    }
    if (index == 1) {
      return y;
    }
    if (index == 2) {
      return z;
    }

    static float dummyData;
    return dummyData;
  }

  /**
   * @brief Overrides [] operator.
   * @param[in] index The Vector3 object you want to get data 
   * from
   * @return The Vector3 x, y , or z index
   */
  float operator[](int index) const {
    if (index == 0) {
      return x;
    }
    if (index == 1) {
      return y;
    }
    if (index == 2) {
      return z;
    }
    return 0.0;
  }

  /**
   * @brief Gets the magnitude of the Vector3
   * 
   * @return The Vector3's magnitude
   */
  [[nodiscard]] float Magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  /**
   * @brief Gets the Unit Vector of this Vector3
   * 
   * @return The Vector3's Unit Vector
   */
  Vector3 Unit() { return Magnitude() == 0 ? (*this) : (*this) / Magnitude(); }

  /**
   * @brief Gets the distance between this Vector3
   * and another Vector3
   * @param v the other Vector3 passed in
   * @return The distance between the two Vector3's
   */
  [[nodiscard]] float Distance(const Vector3 &v) const {
    return (float) sqrt(pow(v.x - this->x, 2) + pow(v.y - this->y, 2) +
        pow(v.z - this->z, 2));
  }

  /**
   * @brief Prints vector to nicely formatted string
   */
  void Print() const;
};

#endif  // Vector3_H_
