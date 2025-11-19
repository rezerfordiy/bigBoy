#include <algorithm>
#include <cmath>

#include "CutFindingTask.hpp"

std::vector<std::pair<Point, Point>> const& CutFindingTask::getPathPoint() const {
    return pathPoint;
}

bool CutFindingTask::process(PathData const& other) {
    data = other;
    std::vector<Point> destinations = data.getDestinations();
    cord step = data.getWorkWidth() / 2;

    cord xMin = std::numeric_limits<cord>::max();
    cord xMax = std::numeric_limits<cord>::lowest();
    cord yMin = std::numeric_limits<cord>::max();
    cord yMax = std::numeric_limits<cord>::lowest();
    
    for (auto const& p : destinations) {
        xMin = std::min(xMin, p.x());
        xMax = std::max(xMax, p.x());
        yMin = std::min(yMin, p.y());
        yMax = std::max(yMax, p.y());
    }

    xMin -= step * 2;
    xMax += step * 2;
    yMin -= step * 2;
    yMax += step * 2;

    auto findIntersections = [&](cord scanValue, bool isHorizontal) {
        std::vector<cord> intersections;
        int n = destinations.size();
        
        for (int i = 0; i < n; i++) {
            Point p1 = destinations[i];
            Point p2 = destinations[(i + 1) % n];
            
            if (isHorizontal) {
                if ((p1.y() <= scanValue && p2.y() >= scanValue) || 
                    (p1.y() >= scanValue && p2.y() <= scanValue)) {
                    if (std::fabs(p1.y() - p2.y()) > 1e-6) {
                        cord x = p1.x() + (scanValue - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
                        intersections.push_back(x);
                    }
                }
            } else {
                if ((p1.x() <= scanValue && p2.x() >= scanValue) || 
                    (p1.x() >= scanValue && p2.x() <= scanValue)) {
                    if (std::fabs(p1.x() - p2.x()) > 1e-6) {
                        cord y = p1.y() + (scanValue - p1.x()) * (p2.y() - p1.y()) / (p2.x() - p1.x());
                        intersections.push_back(y);
                    }
                }
            }
        }
        
        std::sort(intersections.begin(), intersections.end());
        
        intersections.erase(
            std::unique(intersections.begin(), intersections.end(),
                [](cord a, cord b) { return std::fabs(a - b) < 1e-6; }),
            intersections.end());
            
        return intersections;
    };

    for (cord y = yMin; y <= yMax; y += step) {
        auto intersections = findIntersections(y, true);
        
        if (intersections.size() >= 2 && intersections.size() % 2 == 0) {
            for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
                pathPoint.push_back(std::make_pair(
                    Point(intersections[i], y),
                    Point(intersections[i + 1], y)
                ));
            }
        }
    }

    cord extraStep = step;
    for (cord x = xMin; x <= xMax; x += extraStep) {
        auto intersections = findIntersections(x, false);
        
        if (intersections.size() >= 2 && intersections.size() % 2 == 0) {
            for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
                pathPoint.push_back(std::make_pair(
                    Point(x, intersections[i]),
                    Point(x, intersections[i + 1])
                ));
            }
        }
    }

    return true;
}