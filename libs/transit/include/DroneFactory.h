#ifndef DRONE_FACTORY_H_
#define DRONE_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "Drone.h"

/**
 *@brief Drone Factory to produce Drones class.
 **/
class DroneFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for IEntityFactory class.
   **/
  ~DroneFactory() override = default;

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   * null pointer if creation failed.
   **/
  IEntity *CreateEntity(JsonObject &entity) override;
};

#endif
