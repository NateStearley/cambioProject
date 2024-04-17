#ifndef HUMAN_FACTORY_H_
#define HUMAN_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "Human.h"

/**
 *@brief Human Factory to produce Human class.
 **/
class HumanFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for HumanFactory class.
   **/
  ~HumanFactory() override = default;

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   * null pointer if creation failed.
   **/
  IEntity *CreateEntity(JsonObject &entity) override;
};

#endif
