#include "Solver.hpp"

Solver::Solver(std::string const& path) {
    data.loadFromYAML(path);
    switch (data.getTaskType()) {
        case PathData::TaskType::PATHFINDING:
            pathFindingTask.process(data);
            break;
        case PathData::TaskType::CUTFINDING:
            cutFindingTask.process(data);
            break;
        default:
            break;
    }
}

PathData const& Solver::getData() const {
    return data;
}

PathFindingTask const& Solver::getPathFindingTask() const {
    return pathFindingTask;
}

CutFindingTask const& Solver::getTask2() const {
    return cutFindingTask;
}