#ifndef Graph_hpp
#define Graph_hpp

#include "Types.hpp"

class PathData;
class ObstacleManager;

class Graph {
public:
    struct Node {
        int id;
        cord x, y;
        bool isPrimary;
    };

    struct Edge {
        int from, to;
        cord weight;
    };
    
    Graph();
    
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<std::pair<int, cord>>> list;
    std::vector<int> destinationIndexes;
    
    void buildFromVoronoi(VoronoiDiagram const& vd, PathData const* data);
    void clear();
    cord abs(Node const& v1, Node const& v2) const;
    void buildAdjacencyMatrix();
    void buildAdjacencyList();

private:
    cord const MAX_RADIUS = 1000;
    
    void processVoronoiVertices(VoronoiDiagram const& vd,
                               ObstacleManager const& obstacles,
                               std::unordered_map<VoronoiDiagram::vertex_type const*, int>& vertexIdMap,
                               int& vertexId);
    
    void processVoronoiEdges(VoronoiDiagram const& vd,
                            ObstacleManager const& obstacles,
                            std::unordered_map<VoronoiDiagram::vertex_type const*, int>& vertexIdMap);
    
    void processDestionations(std::vector<Point> const& destinations, ObstacleManager const& obstacleManager, int& vertexId);
};

#endif /* Graph_hpp */