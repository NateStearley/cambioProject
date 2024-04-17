#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "routing_strategy.h"
#include "distance_function.h"
#include "bounding_box.h"

namespace routing {

class IGraphNode;
class RoutingStrategy;

class IGraph {
 public:
  virtual ~IGraph() = default;
  [[nodiscard]] virtual const IGraphNode *GetNode(const std::string &name) const = 0;
  [[nodiscard]] virtual const std::vector<IGraphNode *> &GetNodes() const = 0;
  [[nodiscard]] virtual BoundingBox GetBoundingBox() const = 0;
  [[nodiscard]] virtual const IGraphNode *NearestNode(std::vector<float> point,
                                                      const DistanceFunction &distance) const = 0;
  [[nodiscard]] virtual const std::vector<std::vector<float> > GetPath(std::vector<float> src,
                                                                       std::vector<float> dest,
                                                                       const RoutingStrategy &strategy) const = 0;
};

class IGraphNode {
 public:
  virtual ~IGraphNode() = default;
  [[nodiscard]] virtual const std::string &GetName() const = 0;
  [[nodiscard]] virtual const std::vector<IGraphNode *> &GetNeighbors() const = 0;
  [[nodiscard]] virtual const std::vector<float> GetPosition() const = 0;
};

class GraphBase : public IGraph {
 public:
  ~GraphBase() override = default;
  [[nodiscard]] BoundingBox GetBoundingBox() const override;
  [[nodiscard]] const IGraphNode *NearestNode(std::vector<float> point,
                                              const DistanceFunction &distance) const override;
  [[nodiscard]] const std::vector<std::vector<float> > GetPath(std::vector<float> src,
                                                               std::vector<float> dest,
                                                               const RoutingStrategy &strategy) const override;
};

}

#endif
