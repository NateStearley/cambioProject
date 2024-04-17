#include "SimulationModel.h"

#include "DroneFactory.h"
#include "ElectricDroneFactory.h"
#include "HelicopterFactory.h"
#include "HumanFactory.h"
#include "RechargeStationFactory.h"
#include "RobotFactory.h"

SimulationModel::SimulationModel(IController &controller)
    : controller(controller), graph(nullptr) {
  compFactory = new CompositeFactory();
  AddFactory(new DroneFactory());
  AddFactory(new RobotFactory());
  AddFactory(new HumanFactory());
  AddFactory(new HelicopterFactory());
  AddFactory(new RechargeStationFactory());
  AddFactory(new ElectricDroneFactory());
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (auto &entity : entities) {
    delete entity;
  }
  // for (auto &entity : scheduler) {
  //   delete entity;
  // }
  // delete graph;
  graph = nullptr;
  delete compFactory;
}

void SimulationModel::CreateEntity(JsonObject &entity) {
  std::string type = (std::string)entity["type"];
  std::string name = (std::string)entity["name"];
  JsonArray position = (JsonArray)entity["position"];
  std::cout << name << ": " << position << std::endl;

  IEntity *myNewEntity = compFactory->CreateEntity(entity);
  myNewEntity->SetGraph(graph);

  // Call AddEntity to add it to the view
  controller.AddEntity(*myNewEntity);
  entities.push_back(myNewEntity);
}

/// Schedules a trip for an object in the scene
void SimulationModel::ScheduleTrip(JsonObject &details) {
  std::string name = (std::string)details["name"];
  JsonArray start = (JsonArray)details["start"];
  JsonArray end = (JsonArray)details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  for (auto entity : entities) {  // Add the entity to the scheduler
    JsonObject detailsTemp = entity->GetDetails();
    std::string nameTemp = (std::string)detailsTemp["name"];
    std::string typeTemp = (std::string)detailsTemp["type"];
    if (name == nameTemp && typeTemp == "robot" && entity->GetAvailability()) {
      std::string strategyName = (std::string)details["search"];
      entity->SetDestination(Vector3(static_cast<float>(end[0]),
                                     static_cast<float>(end[1]),
                                     static_cast<float>(end[2])));
      entity->SetStrategyName(strategyName);
      scheduler.push_back(entity);
      break;
    }
  }
  controller.SendEventToView("TripScheduled", details);
}

/// Updates the simulation
void SimulationModel::Update(double dt) {
  for (auto &entity : entities) {
    JsonObject dets = entity->GetDetails();
    std::string type = (std::string)dets["type"];
    if (type != "RechargeStation") {
      entity->Update(dt, scheduler);
      controller.UpdateEntity(*entity);
    }
  }
}

void SimulationModel::AddFactory(IEntityFactory *factory) {
  compFactory->AddFactory(factory);
}
