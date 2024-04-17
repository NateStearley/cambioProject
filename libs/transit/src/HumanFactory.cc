#include "HumanFactory.h"

IEntity *HumanFactory::CreateEntity(JsonObject &entity) {
  std::string type = (std::string) entity["type"];
  if (type == "human") {
    std::cout << "Human Created" << std::endl;
    return new Human(entity);
  }
  return nullptr;
}
