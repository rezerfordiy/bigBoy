#include "PathFindingTask.hpp"
#include "AStar.hpp"
#include "RangedDubinsSolver.hpp"

GraphManager const& PathFindingTask::getGraphManager() const {
    return gm;
}

PathData const& PathFindingTask::getData() const {
    return data;
}

std::vector<int> const& PathFindingTask::getPathIndexes() const {
    return pathIndexes;
}

std::vector<DubinsPath> const& PathFindingTask::getPathDubins() const {
    return pathDubins;
}

std::vector<Point> const& PathFindingTask::getPathPoint() const {
    return pathPoint;
}

bool PathFindingTask::process(PathData const& other) {
    data = other;
    gm.build(&data);
    
    auto const& destinations = gm.graph.destinationIndexes;
    
    for (int i = 0; i < destinations.size() - 1; i++) {
        std::vector<int> subpath = AStar()(destinations[i], destinations[i + 1], gm.graph);
        if (subpath.empty()) {
            return 1;
        }
        if (i == 0) {
            pathIndexes.insert(pathIndexes.end(), subpath.begin(), subpath.end());
        } else {
            pathIndexes.insert(pathIndexes.end(), subpath.begin() + 1, subpath.end());
        }
    }
    
    for (int i : pathIndexes) {
        pathPoint.emplace_back(gm.graph.nodes[i].x, gm.graph.nodes[i].y);
    }
    
    if (pathIndexes.size() < 2) return 1;
    
    for (int i = 1; i < pathIndexes.size(); i++) {
        int fromId = pathIndexes[i - 1];
        int toId = pathIndexes[i];
        
        cord x1 = gm.graph.nodes[fromId].x;
        cord y1 = gm.graph.nodes[fromId].y;
        cord x2 = gm.graph.nodes[toId].x;
        cord y2 = gm.graph.nodes[toId].y;
        
        cord dx = x2 - x1;
        cord dy = y2 - y1;
        cord distance = sqrt(dx * dx + dy * dy);
        
        if (distance < 0.1) continue;
        
        cord thetaStart = atan2(dy, dx);
        cord thetaEnd = thetaStart;
        
        if (i + 1 < pathIndexes.size()) {
            int nextId = pathIndexes[i + 1];
            cord x3 = gm.graph.nodes[nextId].x;
            cord y3 = gm.graph.nodes[nextId].y;
            cord dxNext = x3 - x2;
            cord dyNext = y3 - y2;
            if (sqrt(dxNext * dxNext + dyNext * dyNext) > 0.1) {
                thetaEnd = atan2(dyNext, dxNext);
            }
        }
        
        DubinsConfiguration q0 = DubinsConfiguration(x1, y1, thetaStart);
        DubinsConfiguration q1 = DubinsConfiguration(x2, y2, thetaEnd);
        
        auto solver = RangedDubinsSolver(data.getRadius(), data.getRadius() * 10, 5, 0.1, &data.getObstacleManager());
        
        if (auto dubinsPath = solver.shortestPath(q0, q1, -2)) {
            pathDubins.push_back(*dubinsPath.get());
        }
    }
    
    return 0;
}