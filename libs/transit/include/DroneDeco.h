#ifndef DRONE_DECO_H_
#define DRONE_DECO_H_

#include <utility>
#include <vector>

#include "Drone.h"
#include "IEntity.h"
#include "math/vector3.h"

/**
 * @brief A decorator class that wraps a Drone implementation, forwarding all
 * method calls to the host drone except Drone::SetGraph and Drone::Update. The
 * purpose of this class is to indiscriminately override these two functions
 * regardless of the implementation of a given Drone.
 */
class DroneDeco : public Drone {
 public:
  /**
   * @brief Constructs a DroneDeco with the given host drone so that the
   * Drone::SetGraph and Drone::Update methods can be overridden regardless of
   * the given drone's implementation.
   *
   * @param host_drone an instance of drone implementation to wrap
   */
  explicit DroneDeco(Drone *host_drone);

  /**
   * @brief Destructs the host drone
   */
  ~DroneDeco() override;

  /**
   * @brief Gets the host drone this DroneDeco is wrapping.
   *
   * @return the host drone this DroneDeco is wrapping
   */
  Drone *GetHostDrone() { return host_drone; }

  /**
   * @brief Sets the graph object used by this DroneDeco in the simulation.
   *
   * @param graph The IGraph object to be used
   */
  void SetGraph(const routing::IGraph *graph) override;

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, const std::vector<IEntity *> &scheduler) override = 0;

  /**
   * @brief Gets the position of the host drone.
   * @return The position of the host drone.
   */
  [[nodiscard]] Vector3 GetPosition() const override {
    return host_drone->GetPosition();
  }

  /**
   * @brief Gets the direction of the host drone.
   * @return The direction of the host drone.
   */
  [[nodiscard]] Vector3 GetDirection() const override {
    return host_drone->GetDirection();
  }

  /**
   * @brief Gets the destination of the host drone.
   * @return The destination of the host drone.
   */
  [[nodiscard]] Vector3 GetDestination() const override {
    return host_drone->GetDestination();
  }

  /**
   * @brief Gets the details of the host drone.
   * @return The details of the host drone.
   */
  [[nodiscard]] JsonObject GetDetails() const override {
    return host_drone->GetDetails();
  }

  /**
   * @brief Gets the speed of the host drone.
   * @return The speed of the host drone.
   */
  [[nodiscard]] float GetSpeed() const override {
    return host_drone->GetSpeed();
  }

  /**
   * @brief Gets the color of the host drone.
   * @return The color of the host drone.
   */
  [[nodiscard]] std::string GetColor() const override {
    return host_drone->GetColor();
  }

  /**
   * @brief Gets the availability of the drone
   * @return The availability of the drone
   */
  [[nodiscard]] bool GetAvailability() const override {
    return host_drone->GetAvailability();
  }

  /**
   * @brief Gets the nearest entity in the scheduler
   * @param scheduler Vector containing all the entities in the system
   */
  void GetNearestEntity(const std::vector<IEntity *> &scheduler) override {
    host_drone->GetNearestEntity(scheduler);
  }

  /**
   * @brief Sets the position of the drone
   * @param pos_ The new position of the drone
   */
  void SetPosition(Vector3 pos_) override { host_drone->SetPosition(pos_); }

  /**
   * @brief Sets the direction of the drone
   * @param dir_ The new direction of the drone
   */
  void SetDirection(Vector3 dir_) override { host_drone->SetDirection(dir_); }

  /**
   * @brief Sets the destination of the drone
   * @param des_ The new destination of the drone
   */
  void SetDestination(Vector3 des_) override {
    host_drone->SetDestination(des_);
  }

  /**
   * @brief Sets the color of the drone
   * @param col_ The new color of the drone
   */
  void SetColor(const std::string &col_) override {
    host_drone->SetColor(col_);
  }

  /**
   * @brief Rotates the drone
   * @param angle The angle by which the drone should be rotated
   */
  void Rotate(double angle) override { host_drone->Rotate(angle); }

  /**
   * @brief Makes the drone jump
   * @param height The height at which the drone should jump
   */
  void Jump(double height) override { host_drone->Jump(height); }

 protected:
  /**
   * @brief Determines if all of the drones graphs
   * variables have been set
   */
  bool droneGraph = false;

  /**
   * @brief Host_drone for the DroneDeco
   */
  Drone *host_drone = nullptr;
};

#endif
