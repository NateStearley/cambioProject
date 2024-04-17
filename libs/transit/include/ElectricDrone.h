#ifndef DBATTERYD_DECO_H_
#define DBATTERYD_DECO_H_

#include "DataCollection.h"
#include "DroneDeco.h"
#include "RechargeStation.h"

/**
 * @brief The state an ElectricDrone is currently in
 */
enum DroneState {
  /**
   * @brief This ElectricDrone is currently waiting to accept a job at a
   * recharge station with a full battery.
   */
  WaitingAtRechargeStation,
  /**
   * @brief This ElectricDrone has run out of battery
   */
  Dead,
  /**
   * @brief This ElectricDrone is currently transporting a passenger to its
   * destination
   */
  TransportingPassenger,
  /**
   * @brief This ElectricDrone has just completed a trip and is returning to a
   * RechargeStation
   */
  ReturningToRechargeStation,
  /**
   * @brief This Drone is currently recharging at a recharge station.
   */
  Recharging
};

/**
 * @brief A DroneDeco implementation that adds battery functionality to a Drone.
 * All trips that an ElectricDrone will accept will not cause it to run out of
 * battery. The actual implementation of travelling to, picking up, and dropping
 * off passengers is left to the given host drone implementation.
 */
class ElectricDrone : public DroneDeco {
 public:
  /**
   * @brief Creates an ElectricDrone with the given drone which wraps the given
   * drone's functionality with battery-awareness.
   *
   * @param drone the host drone to wrap
   */
  explicit ElectricDrone(Drone *drone);

  /**
   * @brief Destructs the host drone and other internal members.
   */
  ~ElectricDrone() override;

  /**
   * @brief Gets the battery amount.
   * @return The batter amount for the hostDrone
   */
  [[nodiscard]] float GetBattery() const { return this->battery; }

  /**
   * @brief Sets the battery amount for the host_drone
   * @param charge a new charge for the battery
   */
  void SetBattery(float charge) { this->battery = charge; }

  /**
   * @brief Updates the entity's position in the physical system. This
   * overridden implementation only allows the host drone to accept a trip if
   * this ElectricDrone has enough battery to complete the trip. If this drone
   * does not have enough charge, it is navigated to the nearest recharge
   * station. If this ElectricDrone has enough charge, it will use the host
   * drone's Drone::Update implementation to complete the trip, and afterward it
   * will return to the recharge station closest to the destination.
   *
   * @param dt the time in seconds since this method was last called
   * @param scheduler a list containing all the entities in the system
   */
  void Update(double dt, const std::vector<IEntity *> &scheduler) override;

 private:
  DroneTotals inputter;
  float battery;
  // battery charge loss per unit time, gets multiplied by dt in Update
  float depletionRate = 0.7;
  // set a battery efficiency since actual times vary
  // ensures the drone does not die
  float efficiency = 0.90;
  bool firstStation = false;
  RechargeStation *nearestRechargeStation = nullptr;
  IStrategy *toRechargeStation = nullptr;
  DroneState state = DroneState::WaitingAtRechargeStation;
  IEntity *currentRobot = nullptr;

  /**
   * @brief Determines whether this ElectricDrone can successfully make the trip
   * requested by the nearest entity without running out of battery.
   *
   * @param dt the time in seconds since the Drone::Update method was last
   * called
   * @param scheduler a list containing all the entities in the system
   * @return `true` if this ElectricDrone can successfully make the trip
   * requested by the nearest entity, `false` otherwise
   */
  bool IsCurrentTripPossible(double dt,
                             const std::vector<IEntity *> &scheduler);
};

#endif
