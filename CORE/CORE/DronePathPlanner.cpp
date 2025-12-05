#include "DronePathPlanner.hpp"
#include "AStar.hpp"

DronePathPlanner::DronePathPlanner(PathData const& other) : PathPlanner(other) {
    gm.build(&data);

    auto const& destinations = gm.graph.destinationIndexes;
    
    for (int i = 0; i < destinations.size() - 1; i++) {
        std::vector<int> subpath = AStar()(destinations[i], destinations[i + 1], gm.graph);
        if (subpath.empty()) {
            return;
        }
        if (i == 0) {
            pathIndexes.insert(pathIndexes.end(), subpath.begin(), subpath.end());
        } else {
            pathIndexes.insert(pathIndexes.end(), subpath.begin() + 1, subpath.end());
        }
    }
    
    for (int i : pathIndexes) {
        points.emplace_back(gm.graph.nodes[i].x, gm.graph.nodes[i].y);
    }

}

std::vector<Point> const& DronePathPlanner::getPath() const  {
    return points;
}
