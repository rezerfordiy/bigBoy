#ifndef DronePathPlanner_hpp
#define DronePathPlanner_hpp


#include "GraphManager.hpp"
#include "PathPlanner.hpp"

class DronePathPlanner : public PathPlanner {
public:
    DronePathPlanner(PathData const& data); 
    virtual std::vector<Point> const& getPath() const override;
    GraphManager const& getGraphManager() const {return gm;}
protected:
    GraphManager gm;
    std::vector<int> pathIndexes;
};

#endif /* DronePathPlanner_hpp */