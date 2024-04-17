#include "JumpDecorator.h"

void JumpDecorator::Move(IEntity *const entity, const double dt) {
  if (strategy->IsCompleted() && !IsCompleted()) {
    entity->Jump(dt * 10);
    time += static_cast<float>(dt);
  } else {
    strategy->Move(entity, dt);
  }
}
