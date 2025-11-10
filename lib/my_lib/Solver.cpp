#include "Solver.hpp"


Solver::Solver(std::string const& path) {
    data.loadFromYAML(path);
    switch (data.getTaskType())
    {
    case 1:
        task1.process(data);
        break;
    case 2:
        task2.process(data);
        break;
    default:
        break;
    }
}