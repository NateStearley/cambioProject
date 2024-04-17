#include "ElectricDroneFactory.h"

#include "ChargingStationRegistry.h"

IEntity *ElectricDroneFactory::CreateEntity(JsonObject &entity) {
  std::string type = (std::string)entity["type"];
  if (type == "Electricdrone") {
    std::cout << "ElectricDrone Created" << std::endl;
    Drone *new_drone = new ElectricDrone(new Drone(entity));
    new_drone->SetPosition(
        RechargeStationRegistry::getInstance()->randomStationPosition());
    return new_drone;
  }
  return nullptr;
}
