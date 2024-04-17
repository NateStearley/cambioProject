#ifndef GRAPH_FACTORY_H_
#define GRAPH_FACTORY_H_

#include "graph.h"

namespace routing {

class IGraphFactory {
public:
	virtual ~IGraphFactory() = default;
	[[nodiscard]] virtual IGraph* Create(const std::string& file) const = 0;
};

}

#endif
