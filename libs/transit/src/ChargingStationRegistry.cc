#include "../include/ChargingStationRegistry.h"

#include <random>

RechargeStationRegistry *RechargeStationRegistry::getInstance() {
  static auto *instance = new RechargeStationRegistry();

  return instance;
}

RechargeStation *RechargeStationRegistry::getNearestRechargeStation(
    Vector3 position) const {
  int64_t minimum_distance = std::numeric_limits<int64_t>::max();
  RechargeStation *closest_recharge_station = nullptr;

  for (auto recharge_station : this->recharge_stations) {
    const int64_t distance =
        static_cast<int64_t>(recharge_station->
                            GetPosition().Distance(position));

    if (distance < minimum_distance) {
      minimum_distance = distance;
      closest_recharge_station = recharge_station;
    }
  }

  return closest_recharge_station;
}

void RechargeStationRegistry::addRechargeStation(RechargeStation *newStation) {
  recharge_stations.push_back(newStation);
}

Vector3 RechargeStationRegistry::randomStationPosition() const {
  if (recharge_stations.empty()) {
    return {};
  }

  std::random_device random_device;
  std::mt19937 random_number_generator(random_device());
  std::uniform_int_distribution<std::mt19937::result_type>
      uniform_recharge_station_index_generator(0, recharge_stations.size() - 1);

  const int64_t random_index =
      uniform_recharge_station_index_generator(random_number_generator);
  return this->recharge_stations[random_index]->GetPosition();
}
