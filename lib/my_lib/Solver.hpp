#ifndef Solver_hpp
#define Solver_hpp

#include <string>

#include "Data.hpp"
#include "Task1.hpp"
#include "Task2.hpp"

class Solver {
public:
    Solver(std::string const& path);
    Data data;
    Task1 task1;
    Task2 task2;
};




#endif /* Solver_hpp */
