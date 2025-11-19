#ifndef PathFindingTask_hpp
#define PathFindingTask_hpp

#include <string>

#include "GraphManager.hpp"
#include "PathData.hpp"
#include "Dubins.hpp"

class PathFindingTask {
public:
    bool process(PathData const& other);
    
    GraphManager const& getGraphManager() const;
    PathData const& getData() const;
    std::vector<int> const& getPathIndexes() const;
    std::vector<DubinsPath> const& getPathDubins() const;
    std::vector<Point> const& getPathPoint() const;

private:
    std::vector<int> pathIndexes;
    std::vector<Point> pathPoint;
    std::vector<DubinsPath> pathDubins;
    PathData data;
    GraphManager gm;
};

#endif /* PathFindingTask_hpp */