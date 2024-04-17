#ifndef RECHARGE_STATION_FACTORY_H_
#define RECHARGE_STATION_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "RechargeStation.h"

/**
 *@brief RechargeStation Factory to produce RechargeStation class.
 **/
class RechargeStationFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for IEntityFactory class.
   **/
  ~RechargeStationFactory() override = default;

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *   if creation failed.
   **/
  IEntity *CreateEntity(JsonObject &entity) override;
};

#endif
