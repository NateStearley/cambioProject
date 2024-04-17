#ifndef HUMAN_H_
#define HUMAN_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * An IEntity implementation that functions as a human.
 */
class Human : public IEntity {
 public:
  /**
   * @brief Human are created with a name
   * @param obj JSON object containing the drone's information
   */
  explicit Human(JsonObject &obj);

  /**
   * @brief Destroy the Human object
   */
  ~Human() override;
  /**
   * @brief Gets the speed of the Human
   * @return The speed of the Human
   */
  [[nodiscard]] float GetSpeed() const override { return speed; }

  /**
   * @brief Gets the position of the Human
   * @return The position of the Human
   */
  [[nodiscard]] Vector3 GetPosition() const override { return position; }

  /**
   * @brief Gets the direction of the Human
   * @return The direction of the Human
   */
  [[nodiscard]] Vector3 GetDirection() const override { return direction; }

  /**
   * @brief Gets the destination of the Human
   * @return The destination of the Human
   */
  [[nodiscard]] Vector3 GetDestination() const override { return destination; }

  /**
   * @brief Gets the details information of the Human
   * @return The details information of the Human
   */
  [[nodiscard]] JsonObject GetDetails() const override { return details; }

  /**
   * @brief Updates the Human's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, const std::vector<IEntity *> &scheduler) override;

  /**
   * @brief Sets the position of the Human
   * @param pos_ The new position of the Human
   */
  void SetPosition(Vector3 pos_) override { position = pos_; }

  /**
   * @brief Sets the direction of the Human
   * @param dir_ The new direction of the Human
   */
  void SetDirection(Vector3 dir_) override { direction = dir_; }

  /**
   * @brief Sets the destination of the Human
   * @param des_ The new destination of the Human
   */
  void SetDestination(Vector3 des_) override { destination = des_; }

  /**
   * Select a random new destination and start travelling to it using the A*
   * pathing algorithm.
   */
  void CreateNewDestination();

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  float speed;
  IStrategy *toDestination = nullptr;
};

#endif
