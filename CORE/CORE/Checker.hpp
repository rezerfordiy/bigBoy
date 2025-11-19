#ifndef Checker_hpp
#define Checker_hpp

#include "Types.hpp"

class Checker {
public:
    virtual bool check(cord x, cord y) const = 0;
    virtual ~Checker() = default;
};

#endif /* Checker_hpp */