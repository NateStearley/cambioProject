#include "Drone.h"

#include <cmath>
#include <limits>

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "JumpDecorator.h"
#include "SpinDecorator.h"
Drone::Drone(JsonObject &obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {static_cast<float>(pos[0]), static_cast<float>(pos[1]),
              static_cast<float>(pos[2])};
  JsonArray dir(obj["direction"]);
  direction = {static_cast<float>(dir[0]), static_cast<float>(dir[1]),
               static_cast<float>(dir[2])};

  speed = static_cast<float>(obj["speed"]);

  available = true;
  pickedUp = false;
}

Drone::~Drone() {
  // Delete dynamically allocated variables
  graph = nullptr;
  delete nearestEntity;
  delete toRobot;
  delete toFinalDestination;
}

void Drone::GetNearestEntity(const std::vector<IEntity *> &scheduler) {
  float minDis = std::numeric_limits<float>::max();
  for (auto entity : scheduler) {
    if (entity->GetAvailability()) {
      float disToEntity = this->position.Distance(entity->GetPosition());
      if (disToEntity <= minDis) {
        minDis = disToEntity;
        nearestEntity = entity;
      }
    }
  }

  if (nearestEntity) {
    // set availability to the nearest entity
    nearestEntity->SetAvailability(false);
    available = false;
    pickedUp = false;

    destination = nearestEntity->GetPosition();
    Vector3 finalDestination = nearestEntity->GetDestination();

    toRobot = new BeelineStrategy(position, destination);

    std::string strategy_name = nearestEntity->GetStrategyName();
    if (strategy_name == "astar")
      toFinalDestination = new JumpDecorator(
          new AstarStrategy(destination, finalDestination, graph));
    else if (strategy_name == "dfs")
      toFinalDestination = new SpinDecorator(new JumpDecorator(
          new DfsStrategy(destination, finalDestination, graph)));
    else if (strategy_name == "dijkstra")
      toFinalDestination = new JumpDecorator(new SpinDecorator(
          new DijkstraStrategy(destination, finalDestination, graph)));
    else
      toFinalDestination = new BeelineStrategy(destination, finalDestination);
  }
}

void Drone::Update(const double dt, const std::vector<IEntity *> &scheduler) {
  if (available) GetNearestEntity(scheduler);

  if (toRobot) {
    toRobot->Move(this, dt);

    if (toRobot->IsCompleted()) {
      delete toRobot;
      toRobot = nullptr;
      pickedUp = true;
    }
  } else if (toFinalDestination) {
    toFinalDestination->Move(this, dt);

    if (nearestEntity && pickedUp) {
      nearestEntity->SetPosition(position);
      nearestEntity->SetDirection(direction);
    }

    if (toFinalDestination->IsCompleted()) {
      delete toFinalDestination;
      toFinalDestination = nullptr;
      nearestEntity = nullptr;
      available = true;
      pickedUp = false;
    }
  }
}

void Drone::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = static_cast<float>(
      (dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle)));
  direction.z = static_cast<float>(
      (dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle)));
}

void Drone::Jump(double height) {
  if (goUp) {
    position.y += static_cast<float>(height);
    jumpHeight += static_cast<float>(height);
    if (jumpHeight > 5) {
      goUp = false;
    }
  } else {
    position.y -= static_cast<float>(height);
    jumpHeight -= static_cast<float>(height);
    if (jumpHeight < 0) {
      goUp = true;
    }
  }
}
