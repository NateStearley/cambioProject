#include "DroneFactory.h"

IEntity *DroneFactory::CreateEntity(JsonObject &entity) {
  std::string type = (std::string) entity["type"];
  if (type == "drone") {
    std::cout << "Drone Created" << std::endl;
    return new Drone(entity);
  }
  return nullptr;
}
