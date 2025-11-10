#ifndef RangedDubinsSolver_hpp
#define RangedDubinsSolver_hpp

#include "Dubins.hpp"
#include "Checker.hpp"

class RangedDubinsSolver : public DubinsSolver {
public:
    RangedDubinsSolver(cord from, cord to, cord step, cord how_to_sample, Checker const* ch);
    
    virtual std::unique_ptr<DubinsPath> shortestPath(const DubinsConfiguration& q0, const DubinsConfiguration& q1, cord rho) override;
    
protected:
    bool checkPath(const DubinsPath& path) const;

    cord from, to, step, dt;
    Checker const* checker;
};

#endif /* RangedDubinsSolver_hpp */
