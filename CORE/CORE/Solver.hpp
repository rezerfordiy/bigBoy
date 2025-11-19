#ifndef Solver_hpp
#define Solver_hpp

#include <string>

#include "PathData.hpp"
#include "PathFindingTask.hpp"
#include "CutFindingTask.hpp"




class Solver {
public:
    Solver(std::string const& path);
    PathData const& getData() const;
    PathFindingTask const& getPathFindingTask() const;
    CutFindingTask const& getTask2() const;

private:
    PathData data;
    PathFindingTask task1;
    CutFindingTask task2;
};

#endif /* Solver_hpp */