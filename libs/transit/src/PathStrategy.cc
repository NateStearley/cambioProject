#include "PathStrategy.h"

#include <utility>

PathStrategy::PathStrategy(std::vector<std::vector<float>> p)
    : path(std::move(p)), index(0) {}

void PathStrategy::Move(IEntity *entity, double dt) {
  if (IsCompleted()) return;

  Vector3 vi(path[index][0], path[index][1], path[index][2]);
  Vector3 dir = (vi - entity->GetPosition()).Unit();

  entity->SetPosition(
      entity->GetPosition() + dir * entity->GetSpeed() *
      static_cast<float>(dt));
  entity->SetDirection(dir);

  if (entity->GetPosition().Distance(vi) < 4)
    index++;
}

bool PathStrategy::IsCompleted() { return index >= path.size(); }
