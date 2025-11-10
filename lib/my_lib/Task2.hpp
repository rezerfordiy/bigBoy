#ifndef Task2_hpp
#define Task2_hpp

#include <string>

#include "Data.hpp"

// add inheritance 
class Task2 {
public:

    bool process(Data const& other);

    const std::vector<std::pair<Point, Point>>& getPathPoint() const;

private:
    
    std::vector<std::pair<Point, Point>> pathPoint;
    Data data;
};




#endif /* Task2_hpp */
