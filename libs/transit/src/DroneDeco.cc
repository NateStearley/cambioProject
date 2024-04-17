#include "../include/DroneDeco.h"

DroneDeco::DroneDeco(Drone *host_drone) : host_drone(host_drone) {}

DroneDeco::~DroneDeco() {
  delete this->host_drone;
}
void DroneDeco::SetGraph(const routing::IGraph *graph) {
  this->graph = graph;
  this->host_drone->SetGraph(this->graph);
}
