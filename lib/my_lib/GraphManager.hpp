
#ifndef GraphManager_hpp
#define GraphManager_hpp

#include "Graph.hpp"

class Data;

class GraphManager {
public:
    GraphManager();
    int build(Data const* data);

    Graph graph;
};

#endif /* GraphManager_hpp */
