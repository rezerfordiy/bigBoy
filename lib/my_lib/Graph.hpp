#ifndef Graph_hpp
#define Graph_hpp

#include "Types.hpp"

class Data;
class ObstacleManager;
class unordered_map;

class Graph {
public:
    
    const cord MAX_RADIUS = 1000;
    
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
    // [(node1, node2), (node2, node1).... ]
    std::vector<Edge> edges;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<std::pair<int, cord>>> list;
    std::vector<int> destinationIndexes;
    
    void buildFromVoronoi(const VoronoiDiagram& vd, Data const* data);
    
    void clear();
    
    cord abs(const Node& v1, const Node& v2) const;
    void buildAdjacencyMatrix();
    void buildAdjacencyList();
private:
    void processVoronoiVertices(const VoronoiDiagram& vd,
                               const ObstacleManager& obstacles,
                               std::unordered_map<const VoronoiDiagram::vertex_type*, int>& vertexIdMap,
                               int& vertexId);
    
    void processVoronoiEdges(const VoronoiDiagram& vd,
                            const ObstacleManager& obstacles,
                            std::unordered_map<const VoronoiDiagram::vertex_type*, int>& vertexIdMap);
    
    void processDestionations(std::vector<Point> const& destinations, const ObstacleManager& obstacleManager, int& vertexId);
    
//    void addStartEndNodes(cord startX, cord startY, cord endX, cord endY, int& vertexId);
//    
//    void connectStartEndToGraph(int voronCount, cord startX, cord startY, cord endX, cord endY);
};

#endif /* Graph_hpp */
