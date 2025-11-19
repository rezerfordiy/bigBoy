#include <unordered_map>
#include <cmath>

#include "PathData.hpp"
#include "Graph.hpp"

Graph::Graph() {
}

void Graph::clear() {
    nodes.clear();
    edges.clear();
    matrix.clear();
    list.clear();
    destinationIndexes.clear();
}

cord Graph::abs(Graph::Node const& v1, Graph::Node const& v2) const {
    cord dx = v1.x - v2.x;
    cord dy = v1.y - v2.y;
    return std::sqrt(dx * dx + dy * dy);
}

void Graph::buildFromVoronoi(VoronoiDiagram const& vd, PathData const* data) {
    clear();
    
    auto const& obstacleManager = data->getObstacleManager();
    
    std::unordered_map<VoronoiDiagram::vertex_type const*, int> vertexIdMap;
    int vertexId = 0;
    
    processVoronoiVertices(vd, obstacleManager, vertexIdMap, vertexId);
    processVoronoiEdges(vd, obstacleManager, vertexIdMap);
    
    auto const& destinations = data->getDestinations();
    processDestionations(destinations, obstacleManager, vertexId);
        
    buildAdjacencyMatrix();
    buildAdjacencyList();
}

void Graph::processVoronoiVertices(VoronoiDiagram const& vd,
                                  ObstacleManager const& obstacleManager,
                                  std::unordered_map<VoronoiDiagram::vertex_type const*, int>& vertexIdMap,
                                  int& vertexId) {
    for (auto const& vertex : vd.vertices()) {
        Graph::Node v;
        v.id = vertexId;
        v.x = vertex.x();
        v.y = vertex.y();
        v.isPrimary = true;
        
        if (!obstacleManager.check(v.x, v.y)) continue;
        
        nodes.push_back(v);
        vertexIdMap[&vertex] = vertexId;
        vertexId++;
    }
}

void Graph::processVoronoiEdges(VoronoiDiagram const& vd,
                                ObstacleManager const& obstacleManager,
                                std::unordered_map<VoronoiDiagram::vertex_type const*, int>& vertexIdMap) {
    for (auto const& edge : vd.edges()) {
        if (edge.is_finite()) {
            auto const* v0 = edge.vertex0();
            auto const* v1 = edge.vertex1();
            
            if (v0 && v1 && vertexIdMap.count(v0) && vertexIdMap.count(v1)) {
                int fromId = vertexIdMap[v0];
                int toId = vertexIdMap[v1];
                
                cord x1 = nodes[fromId].x;
                cord y1 = nodes[fromId].y;
                cord x2 = nodes[toId].x;
                cord y2 = nodes[toId].y;
                
                if (obstacleManager.checkSegment(x1, y1, x2, y2)) {
                    double distance = abs(nodes[fromId], nodes[toId]);
                                        
                    Graph::Edge e;
                    e.from = fromId;
                    e.to = toId;
                    e.weight = distance;
                    
                    edges.push_back(e);
                }
            }
        }
    }
}

void Graph::processDestionations(std::vector<Point> const& destinations, ObstacleManager const& obstacleManager, int& vertexId) {
    if (destinations.empty()) { return; }
    
    for (Point const& point : destinations) {
        Graph::Node v;
        v.id = vertexId;
        v.x = point.x();
        v.y = point.y();
        v.isPrimary = false;
        
        for (Graph::Node const& node : nodes) {
            double distance = abs(v, node);
            if (distance <= MAX_RADIUS && obstacleManager.checkSegment(v.x, v.y, node.x, node.y)) {
                Graph::Edge e1, e2;
                e1.from = v.id;
                e1.to = node.id;
                e1.weight = distance;
                
                e2.to = v.id;
                e2.from = node.id;
                e2.weight = distance;
                
                edges.push_back(e1);
                edges.push_back(e2);
            }
        }
        
        nodes.push_back(v);
        destinationIndexes.push_back(vertexId);
        vertexId++;
    }
}

void Graph::buildAdjacencyMatrix() {
    int const n = nodes.size();
    matrix.resize(n, std::vector<int>(n, 0));
    
    for (auto const& edge : edges) {
        matrix[edge.from][edge.to] = 1;
        matrix[edge.to][edge.from] = 1;
    }
}

void Graph::buildAdjacencyList() {
    int const n = nodes.size();
    list.resize(n);
    
    for (auto const& edge : edges) {
        list[edge.from].emplace_back(edge.to, edge.weight);
    }
}
