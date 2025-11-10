#include <unordered_map>
#include <cmath>

#include "Data.hpp"
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

cord Graph::abs(const Graph::Node& v1, const Graph::Node& v2) const {
    cord dx = v1.x - v2.x;
    cord dy = v1.y - v2.y;
    return std::sqrt(dx*dx + dy*dy);
}

void Graph::buildFromVoronoi(const VoronoiDiagram& vd, Data const* data) {
    clear();
    
    auto const& obstacleManager = data->getObstacleManager();
    
    std::unordered_map<const VoronoiDiagram::vertex_type*, int> vertexIdMap;
    int vertexId = 0;
    
    processVoronoiVertices(vd, obstacleManager, vertexIdMap, vertexId);
    processVoronoiEdges(vd, obstacleManager, vertexIdMap);
    
    auto const& destinations = data->getDestinations();
    processDestionations(destinations,obstacleManager, vertexId);
        
    buildAdjacencyMatrix();
    buildAdjacencyList();
}

void Graph::processVoronoiVertices(const VoronoiDiagram& vd,
                                  const ObstacleManager& obstacleManager,
                                  std::unordered_map<const VoronoiDiagram::vertex_type*, int>& vertexIdMap,
                                  int& vertexId) {
    for (const auto& vertex : vd.vertices()) {
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

void Graph::processVoronoiEdges(const VoronoiDiagram& vd,
                                const ObstacleManager& obstacleManager,
                                std::unordered_map<const VoronoiDiagram::vertex_type*, int>& vertexIdMap) {
    for (const auto& edge : vd.edges()) {
        if (edge.is_finite()) {
            const auto* v0 = edge.vertex0();
            const auto* v1 = edge.vertex1();
            
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

void Graph::processDestionations(std::vector<Point> const& destinations, const ObstacleManager& obstacleManager, int& vertexId) {
    if (destinations.empty()) {return;}
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
        
        this->destinationIndexes.push_back(vertexId);
        vertexId++;
    }
}


void Graph::buildAdjacencyMatrix() {
    const int n = nodes.size();
    matrix.resize(n, std::vector<int>(n, 0));
    
    for (const auto& edge : edges) {
        matrix[edge.from][edge.to] = 1;
        matrix[edge.to][edge.from] = 1;
    }
}

void Graph::buildAdjacencyList() {
    const int n = nodes.size();
    list.resize(n);
    
    for (const auto& edge : edges) {
        list[edge.from].emplace_back(edge.to, edge.weight);
    }
}

