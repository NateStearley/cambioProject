#include "../include/RechargeStationFactory.h"

#include "../include/ChargingStationRegistry.h"

IEntity *RechargeStationFactory::CreateEntity(JsonObject &entity) {
  std::string type = (std::string)entity["type"];
  if (type == "RechargeStation") {
    std::cout << "RechargeStation Created" << std::endl;
    auto *newStation = new RechargeStation(entity);
    RechargeStationRegistry::getInstance()->addRechargeStation(newStation);
    return newStation;
  }
  return nullptr;
}
