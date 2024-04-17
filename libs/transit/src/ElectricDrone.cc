#include "../include/ElectricDrone.h"

#include "BeelineStrategy.h"
#include "ChargingStationRegistry.h"
#include "Robot.h"
#include "routing/astar.h"
#include "routing/depth_first_search.h"
#include "routing/dijkstra.h"
#include "routing_api.h"

ElectricDrone::ElectricDrone(Drone *drone) : DroneDeco(drone) {
  battery = 100.00;
}

ElectricDrone::~ElectricDrone() {
  this->nearestRechargeStation = nullptr;
  delete this->toRechargeStation;
}

void ElectricDrone::Update(double dt, const std::vector<IEntity *> &scheduler) {
  if (!droneGraph) {
    SetGraph(graph);
    droneGraph = true;
    JsonObject dets = host_drone->GetDetails();
    inputter.name = (std::string)dets["name"];
  }
  if (!firstStation) {
    nearestRechargeStation =
        RechargeStationRegistry::getInstance()->getNearestRechargeStation(
            host_drone->GetPosition());
    firstStation = true;
  }

  switch (this->state) {
    case WaitingAtRechargeStation:
      inputter.distTrav = 0;
      inputter.batteryLost = 0;
      inputter.tripTime = 0;
      if (IsCurrentTripPossible(dt, scheduler)) {
        host_drone->GetNearestEntity(scheduler);
        state = TransportingPassenger;
        nearestRechargeStation = nullptr;
      }
      break;

    case TransportingPassenger:
      if (host_drone->GetAvailability()) {
        state = ReturningToRechargeStation;
        nearestRechargeStation =
            RechargeStationRegistry::getInstance()->getNearestRechargeStation(
                host_drone->GetPosition());
        toRechargeStation = new BeelineStrategy(
            host_drone->GetPosition(), nearestRechargeStation->GetPosition());
      } else {
        host_drone->Update(dt, scheduler);
        battery -= depletionRate * static_cast<float>(dt);
        inputter.tripTime += dt;
        if (battery < 0.0) {
          state = Dead;
        }
      }
      break;

    case ReturningToRechargeStation:
      if (toRechargeStation && toRechargeStation->IsCompleted()) {
        delete toRechargeStation;
        toRechargeStation = nullptr;
        state = Recharging;
        inputter.batteryLost = 100.00 - this->GetBattery();
        std::cout << "at recharge station: " << battery << std::endl;
      } else if (toRechargeStation) {
        toRechargeStation->Move(host_drone, dt);
        battery -= depletionRate * static_cast<float>(dt);
        inputter.tripTime += dt;
        if (battery < 0.0) {
          state = Dead;
        }
      }
      break;

    case Recharging:
      // get how much battery it has when it first got there to see how much
      // spent
      if (battery < 100.00) {
        nearestRechargeStation->Recharge(this, static_cast<float>(dt));
      } else {
        state = WaitingAtRechargeStation;
        inputter.rechargeNum += 1;
        inputter.tripNum += 1;
        DataCollection::getInstance()->writeTrip(inputter);
        std::cout << "battery charged: " << battery << std::endl;
      }
      break;

    case Dead:
      Vector3 pos = host_drone->GetPosition();
      pos.y -= 1;
      if (pos.y > 0) {
        host_drone->SetPosition(pos);
      }
      break;
  }
}

bool ElectricDrone::IsCurrentTripPossible(
    const double dt, const std::vector<IEntity *> &scheduler) {
  // Logic to calculate
  // 0.  Determine the closest robot to drone_start_position and closest charger
  // to robot_destination
  // 1.  Set droneOrigin, robotOrigin, robot_destination, and droneDest
  // locations
  // 2.  Determine how far the drone can go on a full battery charge
  //     a.  # of update cycles it will last
  //         battery full / dt (units of delta charge per cycle)
  //     b.  max distance = speed x # of cycles the battery will last
  // 3.  Three travel segments to calculate distance
  //     a. drone_start_position to robot_start_position
  //     b. robot_destination to droneDest
  //        i.  a and b can be calculated with simple beeline
  //     c. if a + b < max distance, then
  //        i.  calculate robot_start_position to robot_destination
  //            - get the robot strategy
  //            - use the drone's graph pointer to getPath from
  //            robot_start_position to robot_destination
  //            - loop through path nodes and calculate distance between each
  //              - by doing a and b first, you can stop if this part exceeds
  //              the
  //              - distance the battery can support with the drone's speed

  IEntity *nearest_entity = nullptr;
  float min_distance = std::numeric_limits<float>::max();
  for (auto entity : scheduler) {
    if (entity->GetAvailability()) {
      float distance_to_entity =
          host_drone->GetPosition().Distance(entity->GetPosition());
      if (distance_to_entity <= min_distance) {
        min_distance = distance_to_entity;
        nearest_entity = entity;
      }
    }
  }
  // checks if nearest_entity is null or is the same from the last time we went
  // through IsCurrentTripPossible
  if (nearest_entity == nullptr || currentRobot == nearest_entity) {
    return false;
  } else {
    currentRobot = nearest_entity;
  }

  // calculate max distance assuming constant current velocity
  const Vector3 drone_start_position = host_drone->GetPosition();
  const Vector3 robot_start_position = nearest_entity->GetPosition();
  const Vector3 robot_destination = nearest_entity->GetDestination();
  const IEntity *const chargingStation =
      RechargeStationRegistry::getInstance()->getNearestRechargeStation(
          robot_destination);
  const Vector3 droneDest = chargingStation->GetPosition();

  // std::cout << "*** calculating if trip can be made... ******* \n";
  // std::cout << "full battery: " << battery << std::endl;
  // std::cout << "speed: " << host_drone->GetSpeed() << std::endl;
  // std::cout << "delta time:  " << dt << std::endl;

  // calculate first and third leg distances
  const float drone_start_to_robot_start =
      drone_start_position.Distance(robot_start_position);
  const float timeA = drone_start_to_robot_start / host_drone->GetSpeed();
  const float robot_destination_to_drone_destination =
      robot_destination.Distance(droneDest);
  const float timeC =
      robot_destination_to_drone_destination / host_drone->GetSpeed();

  // battery depletion estimates for trip segments A and C
  const float depletionA = timeA * depletionRate;
  const float depletionC = timeC * depletionRate;

  // calculation statistics for debug
  // std::cout << "drone to robot trip distance " << drone_start_to_robot_start
  //           << std::endl;
  // std::cout << "drone to robot est battery depletion " << depletionA
  //           << std::endl;
  // std::cout << "drone to robot trip time " << timeA << std::endl <<
  // std::endl; std::cout << "robot to charging trip distance "
  //           << robot_destination_to_drone_destination
  //           << std::endl;
  // std::cout << "robot to charging est battery depletion " << depletionC
  //           << std::endl;
  // std::cout << "robot to charging trip time " << timeC << std::endl
  //           << std::endl;

  // if trip segments A and C deplete the battery, no need to calculate further
  if (battery * efficiency <= depletionA + depletionC) {
    std::cout
        << "Battery depleted from segments A and C so trip cannot be made: "
        << std::boolalpha << false << std::endl
        << std::endl;
    std::cout << "*****************\n";
    return false;
  }

  // if we get here, then we need to check the robot's path distance, e.g. trip
  // segment B
  std::string strategy_name = nearest_entity->GetStrategyName();
  // std::cout << "strategy: " << strategy_name << std::endl;

  // type conversions for segment B robot begin/dest positions
  std::vector<float> robot_beginning_position = {robot_start_position[0],
                                                 robot_start_position[1],
                                                 robot_start_position[2]};
  std::vector<float> robot_destination_position = {
      robot_destination[0], robot_destination[1], robot_destination[2]};

  std::vector<float> start =
      graph->NearestNode(robot_beginning_position, EuclideanDistance())
          ->GetPosition();
  std::vector<float> end =
      graph->NearestNode(robot_destination_position, EuclideanDistance())
          ->GetPosition();

  std::vector<std::vector<float>> pathB;
  if (strategy_name == "dfs")
    pathB = graph->GetPath(start, end, DepthFirstSearch::Default());
  else if (strategy_name == "dijkstra")
    pathB = graph->GetPath(start, end, Dijkstra::Default());
  else if (strategy_name == "astar")
    pathB = graph->GetPath(start, end, AStar::Default());
  else
    throw std::runtime_error("unrecognized strategy name");

  // std::cout << "Path size: " << pathB.size() << std::endl;

  float robotOrigToRobotDest = 0;
  float timeB = 0;
  float depletionB = 0;

  // // calculate the path's distance
  for (int index = 0; index < pathB.size() - 1; ++index) {
    // std::cout << "index: " << index << std::endl;
    Vector3 node(pathB[index][0], pathB[index][1], pathB[index][2]);
    Vector3 nextNode(pathB[index + 1][0], pathB[index + 1][1],
                     pathB[index + 1][2]);

    // update distance
    robotOrigToRobotDest += node.Distance(nextNode);

    // update incremental time for this mini segment between nodes
    timeB += node.Distance(nextNode) / host_drone->GetSpeed();

    // update incremental est. depletion for this mini segment
    depletionB +=
        (node.Distance(nextNode) / host_drone->GetSpeed()) * depletionRate;

    if (battery * efficiency <= depletionA + depletionB + depletionC) {
      return false;
    }
  }

  // std::cout << "robot path distance: " << robotOrigToRobotDest << std::endl;
  // std::cout << "robot path depletion: " << depletionB << std::endl;
  // std::cout << "robot path time: " << timeB << std::endl << std::endl;

  // std::cout << "Trip est battery depletion: "
  //           << depletionA + depletionB + depletionC << std::endl;
  // std::cout << "Trip est distance: "
  //           << drone_start_to_robot_start + robotOrigToRobotDest
  //               + robot_destination_to_drone_destination << std::endl;

  // std::cout << "Trip est time: " << timeA + timeB + timeC << std::endl
  //           << std::endl;

  // std::cout << "full battery: " << battery << std::endl;

  // timeA, timeC, timeB relative to dt in Update are constant times whereas dt
  //  is not constant so the efficiency is needed to ensure the drone does not
  //  die
  const bool is_valid =
      battery * efficiency > depletionA + depletionB + depletionC;
  if (is_valid) {
    inputter.distTrav = drone_start_to_robot_start + robotOrigToRobotDest +
                        robot_destination_to_drone_destination;
    currentRobot = nullptr;
  }
  std::cout << "Can trip be made: " << std::boolalpha << is_valid << std::endl
            << std::endl;
  std::cout << "*****************\n";
  return is_valid;
}
