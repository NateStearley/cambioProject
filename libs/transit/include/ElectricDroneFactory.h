#ifndef ELECTRIC_DRONE_FACTORY_H_
#define ELECTRIC_DRONE_FACTORY_H_

#include <vector>
#include "IEntityFactory.h"
#include "ElectricDrone.h"

/**
 * @brief An IEntityFactory implementation that produces ElectricDrone instances
 * if the given JsonObject has the correct properties.
 **/
class ElectricDroneFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for ElectricDroneFactory class.
   **/
  ~ElectricDroneFactory() override = default;

  /**
   * @brief Creates an ElectricDrone using the given JSON object if it contains
   * the correct properties, specifically, that its `"type"` member is
   * `"Electricdrone"`.
   *
   * @param entity a JsonObject containing information about the ElectricDrone
   * to create
   * @return an ElectricDrone if creation was successful, `nullptr` otherwise
   **/
  IEntity *CreateEntity(JsonObject &entity) override;
};

#endif
