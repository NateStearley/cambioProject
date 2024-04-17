#include <map>
#include <chrono>
#include "WebServer.h"
#include "SimulationModel.h"
#include "routing_api.h"
#include "../libs/transit/include/DataCollection.h"

//--------------------  Controller ----------------------------

/// A Transit Service that communicates with a web page through web sockets.  It also acts as the controller
/// in the model view controller pattern.
static bool isRunning = true;

class TransitService : public JsonSession, public IController {
 public:
  explicit TransitService(SimulationModel &model)
      : model(model), start(std::chrono::system_clock::now()), time(0.0) {
    routing::RoutingAPI api;
    routing::IGraph *graph = api.LoadFromFile("libs/routing/data/umn.osm");
    model.SetGraph(graph);
  }

  /// Handles specific commands from the web server
  void ReceiveCommand(const std::string &cmd,
                      JsonObject &data,
                      JsonObject &returnValue) override {
    //std::cout << cmd << ": " << data << std::endl;
    if (cmd == "CreateEntity") {
      model.CreateEntity(data);
    } else if (cmd == "ScheduleTrip") {
      model.ScheduleTrip(data);
    } else if (cmd == "ping") {
      returnValue["response"] = data;
    } else if (cmd == "writeResults") {
      DataCollection::getInstance()->writeResults();
      isRunning = false;
      std::cout << "Shutdown started...\n";
    } else if (cmd == "Update") {
      updateEntites.clear();
    
      std::chrono::time_point<std::chrono::system_clock>
          end = std::chrono::system_clock::now();
      std::chrono::duration<double> diff = end - start;
      double delta = diff.count() - time;
      time += delta;

      double simSpeed = (double) data["simSpeed"];
      delta *= simSpeed;

      if (delta > 0.1) {
        for (float f = 0.0; f < delta; f += 0.01) {
          model.Update(0.01);
        }
      } else {
        model.Update(delta);
      }

      for (auto &updateEntity : updateEntites) {
        // Send updated entities
        SendEntity("UpdateEntity", *updateEntity.second, false);
      }
    }
  }

  void SendEntity(const std::string &event,
                  const IEntity &entity,
                  bool includeDetails) {
    //JsonObject details = entity.GetDetails();
    JsonObject details;
    if (includeDetails) {
      details["details"] = entity.GetDetails();
    }
    details["id"] = entity.GetId();
    Vector3 pos_ = entity.GetPosition();
    Vector3 dir_ = entity.GetDirection();
    JsonArray
        pos = {(JsonValue) pos_.x, (JsonValue) pos_.y, (JsonValue) pos_.z};
    JsonArray
        dir = {(JsonValue) dir_.x, (JsonValue) dir_.y, (JsonValue) dir_.z};
    details["pos"] = pos;
    details["dir"] = dir;
    std::string col_ = entity.GetColor();
    if (col_ != "None") details["color"] = col_;
    SendEventToView(event, details);
  }

  void AddEntity(const IEntity &entity) override {
    SendEntity("AddEntity", entity, true);
  }

  void UpdateEntity(const IEntity &entity) override {
    updateEntites[entity.GetId()] = &entity;
  }

  void RemoveEntity(int id) override {
    JsonObject details;
    details["id"] = id;
    SendEventToView("RemoveEntity", details);
  }

  void AddPath(int id, const std::vector<std::vector<float> > &path) override {
    JsonObject details;
    JsonArray array = (JsonArray) details["path"];
    array.Resize((int) path.size());
    for (int i = 0; i < path.size(); i++) {
      JsonArray point = (JsonArray) array[i];
      point.Resize(3);
      point[0] = path[i][0];
      point[1] = path[i][1];
      point[2] = path[i][2];
    }
    SendEventToView("AddPath", details);
  }

  void RemovePath(int id) override {
    JsonObject details;
    details["id"] = id;
    SendEventToView("RemovePath", details);
  }

  /// Allows messages to be passed back to the view
  void SendEventToView(const std::string &event,
                       const JsonObject &details) override {
    JsonObject eventData;
    eventData["event"] = event;
    eventData["details"] = details;
    sendMessage(eventData.ToString());
  }

 private:
  // Simulation Model
  SimulationModel &model;
  // Used for tracking time since last update
  std::chrono::time_point<std::chrono::system_clock> start;
  // The total time the server has been running.
  double time;
  // Current entities to update
  std::map<int, const IEntity *> updateEntites;
};


//--------------------  View / Web Server Code ----------------------------

/// The TransitWebServer holds the simulation and updates sessions.
class TransitWebServer : public WebServerBase, public IController {
 public:
  explicit TransitWebServer(int port = 8081, const std::string &webDir = ".")
      : WebServerBase(port, webDir), model(*this) {}
  void AddEntity(const IEntity &entity) override {
    for (auto &session : sessions) {
      dynamic_cast<TransitService *>(session)->AddEntity(entity);
    }
  }

  void UpdateEntity(const IEntity &entity) override {
    for (auto &session : sessions) {
      dynamic_cast<TransitService *>(session)->UpdateEntity(entity);
    }
  }

  void RemoveEntity(int id) override {
    for (auto &session : sessions) {
      dynamic_cast<TransitService *>(session)->RemoveEntity(id);
    }
  }

  void AddPath(int id, const std::vector<std::vector<float> > &path) override {
    for (auto &session : sessions) {
      dynamic_cast<TransitService *>(session)->AddPath(id, path);
    }
  }

  void RemovePath(int id) override {
    for (auto &session : sessions) {
      dynamic_cast<TransitService *>(session)->RemovePath(id);
    }
  }

  void SendEventToView(const std::string &event,
                       const JsonObject &details) override {
    for (auto &session : sessions) {
      dynamic_cast<TransitService *>(session)->SendEventToView(event,
                                                               details);
    }
  }

 protected:
  Session *createSession() override { return new TransitService(model); }
 private:
  SimulationModel model;
};

/// The main program that handels starting the web sockets service.
int main(int argc, char **argv) {
  if (argc > 1) {
    int port = std::atoi(argv[1]);
    std::string webDir = std::string(argv[2]);
    TransitWebServer server(port, webDir);
    while (isRunning) {
      server.service();
    }
    std::cout << "Exiting...\n";

  } else {
    std::cout
        << "Usage: ./build/bin/transit_service <port> apps/transit_service/web/"
        << std::endl;
  }

  std::cout << "Simulation Exited" << std::endl;
  return 0;
}

