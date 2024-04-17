#include "Robot.h"

Robot::Robot(JsonObject &obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {static_cast<float>(pos[0]), static_cast<float>(pos[1]),
              static_cast<float>(pos[2])};
  JsonArray dir(obj["direction"]);
  direction = {static_cast<float>(dir[0]), static_cast<float>(dir[1]),
               static_cast<float>(dir[2])};
  speed = static_cast<float>(obj["speed"]);
  available = true;
}

JsonObject Robot::GetDetails() const { return details; }

void Robot::SetAvailability(bool choice) { available = choice; }

void Robot::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = static_cast<float>(
      (dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle)));
  direction.z = static_cast<float>(
      (dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle)));
}
