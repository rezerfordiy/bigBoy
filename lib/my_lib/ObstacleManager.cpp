
#include "ObstacleManager.hpp"


void ObstacleManager::add(cord x1, cord y1, cord x2, cord y2) {
    obstacles.emplace_back(x1, y1, x2, y2);
}

std::vector<Obstacle> const& ObstacleManager::get() const {
    return obstacles;
}

bool ObstacleManager::check(cord x, cord y) const {
    bool insideObstacle = false;
    for (const auto& obstacle : obstacles) {
        if (obstacle.isPointInside(x, y)) {
            insideObstacle = true;
            break;
        }
    }
    return !insideObstacle;
}

bool ObstacleManager::checkSegment(cord x1, cord y1, cord x2, cord y2) const {
    bool intersectsObstacle = false;
    for (const auto& obstacle : obstacles) {
        if (obstacle.doesSegmentIntersect(x1, y1, x2, y2)) {
            intersectsObstacle = true;
            break;
        }
    }
    return !intersectsObstacle;
}
