#include "Solver.hpp"

Solver::Solver(std::string const& path) {
    data.loadFromYAML(path);
    switch (data.getTaskType()) {
        case PathData::TaskType::PATHFINDING:
            task1.process(data);
            break;
        case PathData::TaskType::CUTFINDING:
            task2.process(data);
            break;
        default:
            break;
    }
}

PathData const& Solver::getData() const {
    return data;
}

PathFindingTask const& Solver::getPathFindingTask() const {
    return task1;
}

CutFindingTask const& Solver::getTask2() const {
    return task2;
}