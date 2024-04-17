#include "RobotFactory.h"

IEntity *RobotFactory::CreateEntity(JsonObject &entity) {
  std::string type = (std::string)entity["type"];
  if (type == "robot") {
    std::cout << "Robot Created" << std::endl;
    return new Robot(entity);
  }
  return nullptr;
}
