#ifndef JUMP_DECORATOR_H_
#define JUMP_DECORATOR_H_

#include "CelebrationDecorator.h"

/**
 * @brief this class inherits from the IStrategy class and is represents
 * a celebration decorator where the entity will celebrate according to it.
 */
class JumpDecorator : public CelebrationDecorator {
 public:
  /**
   * @brief Construct a new Jump Decorator object
   *
   * @param strategy the strategy to decorate onto
   */
  explicit JumpDecorator(IStrategy *strategy)
      : CelebrationDecorator(strategy) {}

  /**
   * @brief Move the entity with the jump behavior for 4 seconds.
   *
   * @param entity Entity to move
   * @param dt Delta Time
   */
  void Move(IEntity *entity, double dt) override;
};

#endif  // JUMP_DECORATOR_H_
