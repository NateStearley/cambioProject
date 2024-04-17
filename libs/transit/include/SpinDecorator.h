#ifndef SPIN_DECORATOR_H_
#define SPIN_DECORATOR_H_

#include "CelebrationDecorator.h"

/**
 * @brief this class inherits from the IStrategy class and is represents
 * a celebration decorator where the entity will celebrate according to it.
 */
class SpinDecorator : public CelebrationDecorator {
 public:
  /**
   * @brief Construct a new Spin Decorator object
   *
   * @param strategy the strategy to decorate onto
   */
  explicit SpinDecorator(IStrategy *strategy)
      : CelebrationDecorator(strategy) {}

  /**
   * @brief Move the entity with the spin behavior for 4 seconds.
   * 
   * @param entity Entity to move
   * @param dt Delta Time
   */
  void Move(IEntity *entity, double dt) override;
};

#endif  // SPIN_DECORATOR_H_
