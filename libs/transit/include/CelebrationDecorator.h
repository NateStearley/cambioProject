#ifndef CELEBRATION_DECORATOR_H_
#define CELEBRATION_DECORATOR_H_

#include "IStrategy.h"

/**
 * @brief this class inherits from the IStrategy class and is represents
 * a celebration decorator where the entity will celebrate according to it.
 */
class CelebrationDecorator : public IStrategy {
 protected:
  /**
   * @brief strategy of the CelebrationDecorator
   */
  IStrategy *strategy;
  /**
   * @brief How long the celebration lasts
   */
  float time;

 public:
  /**
   * @brief Construct a new Celebration Decorator object
   *
   * @param strategy the strategy to decorate onto
   */
  explicit CelebrationDecorator(IStrategy *strategy);

  /**
   * @brief Celebration Destructor
   */
  ~CelebrationDecorator() override;

  /**
   * @brief Move the entity with the behavior as described for 4 seconds.
   * 
   * @param entity Entity to move
   * @param dt Delta Time
   */
  void Move(IEntity *entity, double dt) override = 0;

  /**
   * @brief Check if the movement is completed by checking the time.
   *
   * @return True if complete, false if not complete
   */
  bool IsCompleted() override;
};

#endif  // CELEBRATION_DECORATOR_H_
