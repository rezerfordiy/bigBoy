#ifndef ObstacleManager_hpp
#define ObstacleManager_hpp

#include "Checker.hpp"
#include "Obstacle.hpp"

class ObstacleManager : public Checker {
public:
    void add(cord x1, cord y1, cord x2, cord y2);
    std::vector<Obstacle> const& get() const;
    void clear() { obstacles.clear(); }
    
    virtual bool check(cord x, cord y) const override;
    bool checkSegment(cord x1, cord y1, cord x2, cord y2) const;

protected:
    std::vector<Obstacle> obstacles;
};

#endif /* ObstacleManager_hpp */