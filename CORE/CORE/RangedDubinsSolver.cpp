#include <iostream>

#include "RangedDubinsSolver.hpp"

RangedDubinsSolver::RangedDubinsSolver(cord from, cord to, cord step, cord howToSample, Checker const* ch) 
    : DubinsSolver(), from(from), to(to), step(step), dt(howToSample), checker(ch) {
    if (step == 0.0 || (step > 0.0 && from > to) || (step < 0.0)) {
        std::cerr << "Error looping radius " << std::endl;
    }
}

bool RangedDubinsSolver::checkPath(DubinsPath const& path) const {
    cord length = path.length();
    if (length <= 0) return true;
    
    cord x = 0.0;
    while (x <= length) {
        cord current_t = std::min(x, length);
        DubinsConfiguration q = DubinsSolver::samplePath(path, current_t);
        
        if (checker && !checker->check(q.x, q.y)) {
            return false;
        }
        
        if (x >= length) break;
        x += dt;
        
        if (x > length - dt / 2.0) {
            x = length;
        }
    }
    return true;
}

std::unique_ptr<DubinsPath> RangedDubinsSolver::shortestPath(DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho) {
    std::unique_ptr<DubinsPath> bestPath = nullptr;
    cord bestLength = std::numeric_limits<cord>::max();
    
    for (cord _rho = from; _rho < to; _rho += step) {
        auto maybePath = DubinsSolver::shortestPath(q0, q1, _rho);
        if (maybePath && checkPath(*maybePath)) {
            cord currentLength = maybePath->length();
            if (currentLength < bestLength) {
                bestLength = currentLength;
                bestPath = std::move(maybePath);
            }
        }
    }
    
    return bestPath;
}