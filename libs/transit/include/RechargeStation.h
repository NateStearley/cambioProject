#ifndef RECHARGE_STATION_H
#define RECHARGE_STATION_H

#include <vector>

#include "IEntity.h"
#include "math/vector3.h"

/**
 * @brief The factor by which the battery percentage of a recharged entity
 * increases relative to the time passed.
 */
const int DEFAULT_RECHARGE_SPEED = 6;

/**
 * @brief An IEntity implementation that contains functionality to recharge
 * ElectricDrone instances.
 */
class RechargeStation : public IEntity {
 public:
  /**
   * @brief Constructs a new RechargeStation with the given parameters inside
   * the given JsonObject. The `"position"` field is interpreted as an array of
   * XYZ values as is used to set the initial position of the RechargeStation.
   *
   * @param obj a JSON object containing details on how to construct this
   * RechargeStation
   */
  explicit RechargeStation(JsonObject &obj);

  /**
   * @brief Destructs this RechargeStation's internal members
   */
  ~RechargeStation() override { graph = nullptr; }

  /**
   * @brief Gets the position of the host_drone.
   * @return The position of the host_drone.
   */
  [[nodiscard]] Vector3 GetPosition() const override { return position; }

  /**
   * @brief Gets the direction of the host_drone.
   * @return The direction of the host_drone.
   */
  [[nodiscard]] Vector3 GetDirection() const override { return {}; }

  /**
   * @brief Gets the destination of the host_drone.
   * @return The destination of the host_drone.
   */
  [[nodiscard]] Vector3 GetDestination() const override { return {}; }

  /**
   * @brief Gets the details of the host_drone.
   * @return The details of the host_drone.
   */
  [[nodiscard]] JsonObject GetDetails() const override { return details; }

  /**
   * @brief Gets the speed of the host_drone.
   * @return The speed of the host_drone.
   */
  [[nodiscard]] float GetSpeed() const override { return 0; }

  /**
   * @brief Recharges the given entity if it is an ElectricDrone to the given
   * amount.
   *
   * @param entity the entity to charge
   * @param charge the battery level to set between 0-100
   */
  void Recharge(IEntity *entity, float charge) const;

 private:
  Vector3 position;
  JsonObject details;
  int recharge_speed = DEFAULT_RECHARGE_SPEED;
};

#endif
