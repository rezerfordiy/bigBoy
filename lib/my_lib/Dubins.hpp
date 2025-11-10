#ifndef DUBINS_H
#define DUBINS_H

#include <functional>
#include <memory>
#include <string>

#include "Types.hpp"

enum class DubinsPathType {
    LSL = 0,
    LSR = 1,
    RSL = 2,
    RSR = 3,
    RLR = 4,
    LRL = 5
};

struct DubinsConfiguration {
    cord x;
    cord y;
    cord theta;
    
    DubinsConfiguration(cord x = 0, cord y = 0, cord theta = 0) : x(x), y(y), theta(theta) {}
};

class DubinsPath {
public:
    DubinsPath(const DubinsConfiguration& start, const cord param[3], cord rho, DubinsPathType type);
    
    const DubinsConfiguration& get_start() const { return start; }
    cord get_param(int i) const;
    cord get_rho() const { return rho; }
    DubinsPathType get_type() const { return type; }
    
    cord length() const;
    cord segmentLength(int i) const;
    cord segmentLengthNormalized(int i) const;
    DubinsConfiguration sample(cord t) const;
    DubinsConfiguration endpoint() const;
    std::unique_ptr<DubinsPath> extractSubpath(cord t) const;
    
    static std::string pathTypeToString(DubinsPathType type);
private:
    DubinsConfiguration start;
    cord param[3];
    cord rho;
    DubinsPathType type;

};

class DubinsSolver {
public:
    virtual std::unique_ptr<DubinsPath> shortestPath(const DubinsConfiguration& q0, const DubinsConfiguration& q1, cord rho);
    
    virtual std::unique_ptr<DubinsPath> path(const DubinsConfiguration& q0, const DubinsConfiguration& q1, cord rho, DubinsPathType pathType);

    static DubinsConfiguration samplePath(const DubinsPath& path, cord t);
    
    using SamplingCallback = std::function<int(const DubinsConfiguration&, cord)>;
    static int sampleMany(const DubinsPath& path, cord stepSize, const SamplingCallback& callback);

    virtual ~DubinsSolver() = default;
protected:
    struct IntermediateResults
    {
        cord alpha;
        cord beta;
        cord d;
        cord sa;
        cord sb;
        cord ca;
        cord cb;
        cord c_ab;
        cord d_sq;
    };

    static cord fmodr(cord x, cord y);
    static cord mod2pi(cord theta);
    
    static int calculateIntermediateResults(IntermediateResults* in, const DubinsConfiguration& q0, const DubinsConfiguration& q1, cord rho);
    
    static DubinsConfiguration dubinsSegment(cord t, const DubinsConfiguration& qi, int segmentType);

    virtual int dubinsLSL(const IntermediateResults* in, cord out[3]);
    virtual int dubinsRSR(const IntermediateResults* in, cord out[3]);
    virtual int dubinsLSR(const IntermediateResults* in, cord out[3]);
    virtual int dubinsRSL(const IntermediateResults* in, cord out[3]);
    virtual int dubinsRLR(const IntermediateResults* in, cord out[3]);
    virtual int dubinsLRL(const IntermediateResults* in, cord out[3]);
    virtual int dubinsWord(const IntermediateResults* in, DubinsPathType pathType, cord out[3]);
    
};

namespace DubinsError
{
    constexpr int OK = 0;           // No error
    constexpr int COCONFIGS = 1;    // Colocated configurations
    constexpr int PARAM = 2;        // Path parameterization error
    constexpr int BADRHO = 3;       // Invalid rho value
    constexpr int NOPATH = 4;       // No connection between configurations
}

#endif // DUBINS_H
