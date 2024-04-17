#ifndef DRONE_H_
#define DRONE_H_

#include <utility>
#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

/**
 * @class Drone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class Drone : public IEntity {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  explicit Drone(JsonObject &obj);

  /**
   * @brief Destructor
   */
  ~Drone() override;

  /**
   * @brief Gets the speed of the drone
   * @return The speed of the drone
   */
  [[nodiscard]] float GetSpeed() const override { return speed; }

  /**
   * @brief Gets the position of the drone
   * @return The position of the drone
   */
  [[nodiscard]] Vector3 GetPosition() const override { return position; }

  /**
   * @brief Gets the direction of the drone
   * @return The direction of the drone
   */
  [[nodiscard]] Vector3 GetDirection() const override { return direction; }

  /**
   * @brief Gets the destination of the drone
   * @return The destination of the drone
   */
  [[nodiscard]] Vector3 GetDestination() const override { return destination; }

  /**
   * @brief Gets the color of the drone
   * @return The color of the drone
   */
  [[nodiscard]] std::string GetColor() const override { return color; }

  /**
   * @brief Gets the details information of the drone
   * @return The details information of the drone
   */
  [[nodiscard]] JsonObject GetDetails() const override { return details; }

  /**
   * @brief Gets the availability of the drone
   * @return The availability of the drone
   */
  [[nodiscard]] bool GetAvailability() const override { return available; }

  /**
   * @brief Gets the nearest entity in the scheduler
   * @param scheduler Vector containing all the entities in the system
   */
  virtual void GetNearestEntity(const std::vector<IEntity *> &scheduler);

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, const std::vector<IEntity *> &scheduler) override;

  /**
   * @brief Sets the position of the drone
   * @param pos_ The new position of the drone
   */
  void SetPosition(Vector3 pos_) override { position = pos_; }

  /**
   * @brief Sets the direction of the drone
   * @param dir_ The new direction of the drone
   */
  void SetDirection(Vector3 dir_) override { direction = dir_; }

  /**
   * @brief Sets the destination of the drone
   * @param des_ The new destination of the drone
   */
  void SetDestination(Vector3 des_) override { destination = des_; }

  /**
  * @brief Sets the color of the drone
  * @param col_ The new color of the drone
  */
  void SetColor(const std::string &col_) override { color = col_; }

  /**
   * @brief Rotates the drone
   * @param angle The angle by which the drone should be rotated
   */
  void Rotate(double angle) override;

  /**
   * @brief Makes the drone jump
   * @param height The height at which the drone should jump
   */
  void Jump(double height) override;

  /**
   * @brief Removing the copy constructor and assignment operator
   * so that drones cannot be copied.
   */
  Drone(const Drone &drone) = delete;
  Drone &operator=(const Drone &drone) = delete;

 protected:
  // explicitly define default constructor as protected for decorator classes
  Drone() {}

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  std::string color = "None";  // None means default color
  float jumpHeight = 0;
  bool goUp = true;  // jump helper
  Vector3 destination;
  float speed = 0;
  bool available = true;
  bool pickedUp = false;
  IEntity *nearestEntity = nullptr;
  IStrategy *toRobot = nullptr;
  IStrategy *toFinalDestination = nullptr;
};

#endif
