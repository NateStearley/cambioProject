#ifndef BEELINE_H_
#define BEELINE_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * @brief this class inherits from the IStrategy class and is responsible for
 * generating the beeline that the drone will take.
 */
class BeelineStrategy : public IStrategy {
 public:
  /**
   * @brief Construct a new Beeline Strategy object
   *
   * @param position Starting position
   * @param destination End destination
   */
  BeelineStrategy(Vector3 position, Vector3 destination);

  /**
   * @brief Move toward next position
   *
   * @param entity Entity to move
   * @param dt Delta Time
   */
  void Move(IEntity *entity, double dt) override;

  /**
   * @brief Check if the trip is completed
   *
   * @return True if complete, false if not complete
   */
  bool IsCompleted() override;

 private:
  Vector3 position;
  Vector3 destination;
};
#endif  // BEELINE_H_
