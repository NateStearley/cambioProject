#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>
#include <random>

#include "graph.h"
#include "math/vector3.h"
#include "util/json.h"

using namespace routing;

/**
 * @class IEntity
 * @brief Represents an entity in a physical system.
 *
 * An IEntity object has a unique ID, a position, a direction, a destination,
 * and details. It also has a speed, which determines how fast the entity moves
 * in the physical system. Subclasses of IEntity can override the `Update`
 * function to implement their own movement behavior.
 */
class IEntity {
 public:
  /**
   * @brief Constructor that assigns a unique ID to the entity.
   */
  IEntity() {
    static int currentId = 0;
    id = currentId;
    currentId++;
  }

  /**
   * @brief Virtual destructor for IEntity.
   */
  virtual ~IEntity() { delete graph; }

  /**
   * @brief Gets the ID of the entity.
   * @return The ID of the entity.
   */
  [[nodiscard]] virtual int GetId() const { return id; }

  /**
   * @brief Gets the position of the entity.
   * @return The position of the entity.
   */
  [[nodiscard]] virtual Vector3 GetPosition() const = 0;

  /**
   * @brief Gets the direction of the entity.
   * @return The direction of the entity.
   */
  [[nodiscard]] virtual Vector3 GetDirection() const = 0;

  /**
   * @brief Gets the destination of the entity.
   * @return The destination of the entity.
   */
  [[nodiscard]] virtual Vector3 GetDestination() const = 0;

  /**
   * @brief Gets the details of the entity.
   * @return The details of the entity.
   */
  [[nodiscard]] virtual JsonObject GetDetails() const = 0;

  /**
   * @brief Gets the color of the drone
   * @return The color of the drone
   */
  [[nodiscard]] virtual std::string GetColor() const { return "None"; }

  /**
   * @brief Gets the speed of the entity.
   * @return The speed of the entity.
   */
  [[nodiscard]] virtual float GetSpeed() const = 0;

  /**
   * @brief Gets the availability of the entity.
   * @return The availability of the entity.
   */
  [[nodiscard]] virtual bool GetAvailability() const {}

  /**
   * @brief Get the strategy Name
   *
   * @return strategy name
   */
  [[nodiscard]] virtual std::string GetStrategyName() const {}

  /**
   * @brief Set the Strategy Name
   *
   * @param strategyName_ Strategy name
   */
  virtual void SetStrategyName(const std::string &strategyName_) {}

  /**
   * @brief Sets the availability of the entity.
   * @param choice The desired availability of the entity.
   */
  virtual void SetAvailability(bool choice) {}

  /**
   * @brief Updates the entity's position in the physical system.
   * @param dt The time step of the update.
   * @param scheduler The list of all entities in the system.
   */
  virtual void Update(double dt, const std::vector<IEntity *> &scheduler) {}

  /**
   * @brief Sets the graph object used by the entity in the simulation.
   * @param graph_ The IGraph object to be used.
   */
  virtual void SetGraph(const IGraph *graph_) { this->graph = graph_; }

  /**
   * @brief Sets the position of the entity.
   * @param pos_ The desired position of the entity.
   */
  virtual void SetPosition(Vector3 pos_) {}

  /**
   *@brief Set the direction of the entity.
   *@param dir_ The new direction of the entity.
   */
  virtual void SetDirection(Vector3 dir_) {}

  /**
   *@brief Set the destination of the entity.
   *@param des_ The new destination of the entity.
   */
  virtual void SetDestination(Vector3 des_) {}

  /**
   * @brief Sets the color of the drone
   * @param col_ The new color of the drone
   */
  virtual void SetColor(const std::string &col_) {}

  /**
   * @brief Rotate the entity.
   * @param angle The angle to rotate the entity by.
   */
  virtual void Rotate(double angle) {}

  /**
   * @brief Make the entity jump.
   * @param height The height to make the entity jump.
   */
  virtual void Jump(double height) {}

  /**
   * Generates a uniformly-distributed random number in the given inclusive range
   *
   * @param min the minimum value of the range of numbers to generate
   * @param max the maximum value of the range of the numbers to generate
   * @return a uniformly-distributed random number in the given inclusive range
   */
  virtual double Random(const float min, const float max) {
    std::random_device random_device;
    std::mt19937 random_number_generator(random_device());
    std::uniform_real_distribution<> uniform_distribution(min, max);

    return uniform_distribution(random_number_generator);
  }

 protected:
  /**
   * @brief ID of the entity
   */
  int id;

  /**
   * @brief graph that the Entity uses in the simulation
   */
  const IGraph *graph = nullptr;
};

#endif
