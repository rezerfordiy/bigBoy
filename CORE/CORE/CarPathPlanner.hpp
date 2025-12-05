#ifndef CarPathPlanner_hpp
#define CarPathPlanner_hpp

#include "DronePathPlanner.hpp"

class CarPathPlanner : public DronePathPlanner {
public:
    CarPathPlanner(PathData const& other);
    virtual std::vector<Point> const& getPath() const override;
protected:
    std::vector<Point> m_points;
};

#endif /* CarPathPlanner_hpp */