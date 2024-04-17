#include "Helicopter.h"

#include <cmath>

#include "BeelineStrategy.h"

Helicopter::Helicopter(JsonObject &obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {static_cast<float>(pos[0]), static_cast<float>(pos[1]),
              static_cast<float>(pos[2])};
  JsonArray dir(obj["direction"]);
  direction = {static_cast<float>(dir[0]), static_cast<float>(dir[1]),
               static_cast<float>(dir[2])};

  speed = static_cast<float>(obj["speed"]);
}

Helicopter::~Helicopter() {
  // Delete dynamically allocated variables
  graph = nullptr;
  delete toDestination;
}

void Helicopter::CreateNewDestination() {
  destination = {static_cast<float>(Random(-1400, 1500)), position.y,
                 static_cast<float>(Random(-800, 800))};
  toDestination = new BeelineStrategy(position, destination);
}

void Helicopter::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = static_cast<float>(
      (dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle)));
  direction.z = static_cast<float>(
      (dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle)));
}

void Helicopter::Update(const double dt,
                        const std::vector<IEntity *> &scheduler) {
  if (toDestination) {
    if (toDestination->IsCompleted()) {
      CreateNewDestination();
    } else if (toDestination != nullptr) {
      toDestination->Move(this, dt);
    }
  } else {
    CreateNewDestination();
  }
}
