#include "CompositeFactory.h"

IEntity *CompositeFactory::CreateEntity(JsonObject &entity) {
  for (auto &componentFactory : componentFactories) {
    IEntity *createdEntity = componentFactory->CreateEntity(entity);
    if (createdEntity != nullptr) {
      return createdEntity;
    }
  }
  std::cout << "[!] Error: Type mismatched..." << std::endl;
  return nullptr;
}

void CompositeFactory::AddFactory(IEntityFactory *factoryEntity) {
  componentFactories.push_back(factoryEntity);
}

CompositeFactory::~CompositeFactory() {
  for (auto &componentFactory : componentFactories) {
    delete componentFactory;
  }
}
