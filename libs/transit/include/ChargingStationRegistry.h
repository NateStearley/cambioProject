#ifndef CSCI3081W_TEAM28_LIBS_TRANSIT_INCLUDE_CHARGINGSTATIONREGISTRY_H_
#define CSCI3081W_TEAM28_LIBS_TRANSIT_INCLUDE_CHARGINGSTATIONREGISTRY_H_

#include "RechargeStation.h"

/**
 * @brief Singleton class that stores all RechargeStation instances created by a
 * RechargeStationFactory. Allows for the closest recharge station to a 3D
 * position and for a random 3D position of a recharge station to be queried.
 */
class RechargeStationRegistry {
 public:
  /**
   * @brief Gets the singleton instance of this class.
   *
   * @return the instance of the RechargeStationRegistry singleton
   */
  static RechargeStationRegistry *getInstance();

  /**
   * @brief Finds the nearest recharge station to the given position.
   *
   * @param position the position at which to find the nearest recharge station
   * to
   * @return the nearest recharge station to the given position or `nullptr` if
   * no recharge stations are registered.
   */
  [[nodiscard]] RechargeStation *getNearestRechargeStation(
      Vector3 position) const;

  /**
   * @brief Registers the given recharge station with this registry.
   *
   * @param newStation the new station to register
   */
  void addRechargeStation(RechargeStation *newStation);

  // Delete the copy constructor so the singleton instance
  RechargeStationRegistry(RechargeStationRegistry &other) = delete;

  /**
   * @brief Get the 3D position of a random registered RechargeStation or the
   * origin if no recharge stations are registered.
   *
   * @return the 3D position of a random registered RechargeStation
   */
  [[nodiscard]] Vector3 randomStationPosition() const;

 private:
  RechargeStationRegistry() {}
  ~RechargeStationRegistry() = default;

  std::vector<RechargeStation *> recharge_stations =
      std::vector<RechargeStation *>();
};

#endif  // CSCI3081W_TEAM28_LIBS_TRANSIT_INCLUDE_CHARGINGSTATIONREGISTRY_H_
