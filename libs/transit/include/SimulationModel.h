#ifndef SIMULATION_MODEL_H_
#define SIMULATION_MODEL_H_

#include "CompositeFactory.h"
#include "Drone.h"
#include "IController.h"
#include "IEntity.h"
#include "Robot.h"
#include "graph.h"
using namespace routing;

//--------------------  Model ----------------------------

/// Simulation Model handling the transit simulation.  The model can communicate
/// with the controller.
/**
 * @brief Class SimulationModel handling the transit simulation. it can
 * communicate with the controller
 **/
class SimulationModel {
 public:
  /**
   * @brief Default constructor that create the SimulationModel object
   **/
  explicit SimulationModel(IController &controller);

  /**
   * @brief Destructor
   */
  ~SimulationModel();

  /**
   * @brief Set the Graph for the SimulationModel
   * @param graph_ Type IGraph* contain the new graph for SimulationModel
   **/
  void SetGraph(const IGraph *graph_) { this->graph = graph_; }

  /**
   * @brief Creates a new simulation entity
   * @param entity Type JsonObject contain the entity's reference to decide
   *which entity to create
   **/
  void CreateEntity(JsonObject &entity);

  /**
   * @brief Schedule a trip for an object in the scene
   * @param details Type JsonObject contain the entity's reference to schedule
   *the detail of the trip being scheduled
   **/
  void ScheduleTrip(JsonObject &details);

  /**
   * @brief Update the simulation
   * @param dt Type double contain the time since update was last called.
   **/
  void Update(double dt);

  // Adds a new factory
  /**
   * @brief Add new factory into the simulation
   * @param factory - Factory to add into the simulation.
   */
  void AddFactory(IEntityFactory *factory);

 protected:
  /**
   * @brief controller for the simulation
   */
  IController &controller;

  /**
   * @brief list of all the entities in the simulation
   */
  std::vector<IEntity *> entities;

  /**
   * @brief list of all the entities in the scheduler
   */
  std::vector<IEntity *> scheduler;

  /**
   * @brief graph for the simulation
   */
  const IGraph *graph;

  /**
   * @brief compositeFactory that has all of the
   * other factories
   */
  CompositeFactory *compFactory;
};

#endif
