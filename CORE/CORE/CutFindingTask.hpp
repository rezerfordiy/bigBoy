#ifndef CutFindingTask_hpp
#define CutFindingTask_hpp

#include <string>
#include <vector>

#include "PathData.hpp"

class CutFindingTask {
public:
    bool process(PathData const& other);
    std::vector<std::pair<Point, Point>> const& getPathPoint() const;

private:
    std::vector<std::pair<Point, Point>> pathPoint;
    PathData data;
};

#endif /* CutFindingTask_hpp */