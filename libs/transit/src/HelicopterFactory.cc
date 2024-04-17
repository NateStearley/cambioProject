#include "HelicopterFactory.h"

IEntity *HelicopterFactory::CreateEntity(JsonObject &entity) {
  std::string type = (std::string) entity["type"];
  if (type == "helicopter") {
    std::cout << "Helicopter Created" << std::endl;
    return new Helicopter(entity);
  }
  return nullptr;
}
