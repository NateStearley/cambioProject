#include "../include/RechargeStation.h"

#include "../include/ElectricDrone.h"

RechargeStation::RechargeStation(JsonObject &obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {static_cast<float>(pos[0]), static_cast<float>(pos[1]),
              static_cast<float>(pos[2])};
}

// specify that recharge stations only take in electric drones
void RechargeStation::Recharge(IEntity *entity, const float charge) const {
  auto *const drone = dynamic_cast<ElectricDrone *>(entity);
  if (drone != nullptr) {
    drone->SetBattery(
        std::min(drone->GetBattery() +
                 charge * static_cast<float>(this->recharge_speed), 100.f));
  }
}
