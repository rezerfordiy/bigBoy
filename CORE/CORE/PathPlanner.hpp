#ifndef PathPlanner_hpp
#define PathPlanner_hpp

#include "PathData.hpp"

class PathPlanner {
public:
    PathPlanner(PathData const& other);
    virtual std::vector<Point> const& getPath() const = 0;
    virtual ~PathPlanner() = default;
protected:
    std::vector<Point> points;
    PathData data;
};

#endif /* PathPlanner_hpp */