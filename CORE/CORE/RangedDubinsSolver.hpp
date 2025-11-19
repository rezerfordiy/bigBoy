#ifndef RangedDubinsSolver_hpp
#define RangedDubinsSolver_hpp

#include "Dubins.hpp"
#include "Checker.hpp"

class RangedDubinsSolver : public DubinsSolver {
public:
    RangedDubinsSolver(cord from, cord to, cord step, cord howToSample, Checker const* ch);
    
    virtual std::unique_ptr<DubinsPath> shortestPath(DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho) override;
    
protected:
    bool checkPath(DubinsPath const& path) const;

    cord from, to, step, dt;
    Checker const* checker;
};

#endif /* RangedDubinsSolver_hpp */