#include <iostream>

#include "RangedDubinsSolver.hpp"


RangedDubinsSolver::RangedDubinsSolver(cord from, cord to, cord step, cord how_to_sample, Checker const* ch) : DubinsSolver(), from(from), to(to), step(step), dt(how_to_sample), checker(ch) {
    if (step == 0.0 ||
        (step > 0.0 && from > to) ||
        (step < 0.0)
        ) {
        std::cerr << "Error looping radius " << std::endl;

    }
}

bool RangedDubinsSolver::checkPath(const DubinsPath& path) const {
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
        
        if (x > length - dt/2.0) {
            x = length;
        }
    }
    return true;
}

std::unique_ptr<DubinsPath> RangedDubinsSolver::shortestPath(const DubinsConfiguration& q0, const DubinsConfiguration& q1, cord rho) {
    std::unique_ptr<DubinsPath> best_path = nullptr;
    cord best_length = std::numeric_limits<cord>::max();
    
    for (cord _rho = from; _rho < to; _rho += step) {
        auto maybe_path = DubinsSolver::shortestPath(q0, q1, _rho);
        if (maybe_path && checkPath(*maybe_path)) {
            cord current_length = maybe_path->length();
            if (current_length < best_length) {
                best_length = current_length;
                best_path = std::move(maybe_path);
            }
        }
    }
    
    return best_path;
}