#ifndef AStar_hpp
#define AStar_hpp

#include "vector"

class Graph;

class AStar {
public:
    std::vector<int> operator()(int from, int to, const Graph& graph) const;
};

#endif /* AStar_hpp */
