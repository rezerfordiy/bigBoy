#ifndef GraphManager_hpp
#define GraphManager_hpp

#include "Graph.hpp"

class PathData;

class GraphManager {
public:
    GraphManager();
    int build(PathData const* data);

    Graph graph;
};

#endif /* GraphManager_hpp */