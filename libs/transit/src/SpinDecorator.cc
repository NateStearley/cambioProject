#include "SpinDecorator.h"

void SpinDecorator::Move(IEntity *const entity, const double dt) {
  if (strategy->IsCompleted() && !IsCompleted()) {
    entity->Rotate(dt * 10);
    time += static_cast<float>(dt);
  } else {
    strategy->Move(entity, dt);
  }
}
