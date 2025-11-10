#ifndef Task1_hpp
#define Task1_hpp

#include <string>

#include "GraphManager.hpp"
#include "Data.hpp"
#include "Dubins.hpp"


class Task1 {
public:

    bool process(Data const& other);

    
    const GraphManager& getGraphManager() const;
    const Data& getData() const;
    const std::vector<int>& getPathIndexes() const;
    const std::vector<DubinsPath>& getPathDubins() const;
    const std::vector<Point>& getPathPoint() const;


private:
    
    std::vector<int> pathIndexes;
    std::vector<Point> pathPoint;
    std::vector<DubinsPath> pathDubins;
    Data data;
    GraphManager gm;
};




#endif /* Task1_hpp */
