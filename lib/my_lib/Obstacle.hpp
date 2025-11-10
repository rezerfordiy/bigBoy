
#ifndef Obstacle_hpp
#define Obstacle_hpp

#include <vector>

#include "Types.hpp"

struct Obstacle {
    static cord dt;
    
    cord x1, y1, x2, y2;
    Obstacle(cord, cord, cord, cord);
    
        std::vector<Point> getPoints() const;
    
    bool doesSegmentIntersect(cord x1, cord y1, cord x2, cord y2) const;
        
    bool isPointInside(cord x, cord y) const;
        
private:
        bool checkLineSegmentIntersection(cord, cord, cord, cord,
                                          cord, cord, cord, cord) const;
    
};


#endif /* Obstacle_hpp */
