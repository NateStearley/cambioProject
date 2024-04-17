#ifndef Helicopter_FACTORY_H_
#define Helicopter_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "Helicopter.h"

/**
 *@brief Helicopter Factory to produce Helicopter class.
 **/
class HelicopterFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for HelicopterFactory class.
   **/
  ~HelicopterFactory() override = default;

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   * nullptr if creation failed.
   **/
  IEntity *CreateEntity(JsonObject &entity) override;
};

#endif
