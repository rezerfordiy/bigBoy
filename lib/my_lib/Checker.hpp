#ifndef checker_hpp
#define checker_hpp

#include "Types.hpp"

class Checker {
public:
    virtual bool check(cord x, cord y) const = 0;
    virtual ~Checker() = default;
};

#endif /* checker_hpp */
