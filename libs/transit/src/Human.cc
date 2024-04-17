#include "Human.h"

#include "AstarStrategy.h"

Human::Human(JsonObject &obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {static_cast<float>(pos[0]), static_cast<float>(pos[1]),
              static_cast<float>(pos[2])};
  JsonArray dir(obj["direction"]);
  direction = {static_cast<float>(dir[0]), static_cast<float>(dir[1]),
               static_cast<float>(dir[2])};

  speed = static_cast<float>(obj["speed"]);
}

Human::~Human() {
  // Delete dynamically allocated variables
  graph = nullptr;
  delete toDestination;
}

void Human::CreateNewDestination() {
  destination = {Random(-1400, 1500), position.y, Random(-800, 800)};
  toDestination = new AstarStrategy(position, destination, graph);
}

void Human::Update(const double dt, const std::vector<IEntity *> &scheduler) {
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
