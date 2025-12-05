
#include "CarPathPlanner.hpp"
#include "RangedDubinsSolver.hpp"

CarPathPlanner::CarPathPlanner(PathData const& other) : DronePathPlanner(other) {
    if (pathIndexes.size() < 2) return;
    
    std::vector<DubinsPath> pathDubins;

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
    constexpr static int AMOUNT = 100;
    for (int i = 0; i < pathDubins.size(); i++) {
        DubinsPath dubin = pathDubins[i];
        cord length = dubin.length();
        if (length > 0) {
            for (int j = 0; j < AMOUNT; j++) {
                cord t = (length * j) / AMOUNT;      
                DubinsConfiguration conf = dubin.sample(t);
                m_points.emplace_back(conf.x, conf.y);
            }
        }
    }
    
}

std::vector<Point> const& CarPathPlanner::getPath() const  {
    return m_points;
}
