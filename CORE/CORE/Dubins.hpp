#ifndef Dubins_hpp
#define Dubins_hpp

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
    DubinsPath(DubinsConfiguration const& start, cord const param[3], cord rho, DubinsPathType type);
    
    DubinsConfiguration const& getStart() const { return start; }
    cord getParam(int i) const;
    cord getRho() const { return rho; }
    DubinsPathType getType() const { return type; }
    
    cord length() const;
    cord segmentLength(int i) const;
    cord segmentLengthNormalized(int i) const;
    DubinsConfiguration sample(cord t) const;
    DubinsConfiguration endPoint() const;
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
    virtual std::unique_ptr<DubinsPath> shortestPath(DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho);
    virtual std::unique_ptr<DubinsPath> path(DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho, DubinsPathType pathType);
    
    static DubinsConfiguration samplePath(DubinsPath const& path, cord t);
    
    using SamplingCallback = std::function<int(DubinsConfiguration const&, cord)>;
    static int sampleMany(DubinsPath const& path, cord stepSize, SamplingCallback const& callback);
    
    virtual ~DubinsSolver() = default;

protected:
    struct IntermediateResults {
        cord alpha;
        cord beta;
        cord d;
        cord sa;
        cord sb;
        cord ca;
        cord cb;
        cord cAb;
        cord dSq;
    };

    static cord fmodr(cord x, cord y);
    static cord mod2pi(cord theta);
    static int calculateIntermediateResults(IntermediateResults* in, DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho);
    static DubinsConfiguration dubinsSegment(cord t, DubinsConfiguration const& qi, int segmentType);
    
    virtual int dubinsLSL(IntermediateResults const* in, cord out[3]);
    virtual int dubinsRSR(IntermediateResults const* in, cord out[3]);
    virtual int dubinsLSR(IntermediateResults const* in, cord out[3]);
    virtual int dubinsRSL(IntermediateResults const* in, cord out[3]);
    virtual int dubinsRLR(IntermediateResults const* in, cord out[3]);
    virtual int dubinsLRL(IntermediateResults const* in, cord out[3]);
    virtual int dubinsWord(IntermediateResults const* in, DubinsPathType pathType, cord out[3]);
};

namespace DubinsError {
    constexpr int OK = 0;
    constexpr int COCONFIGS = 1;
    constexpr int PARAM = 2;
    constexpr int BADRHO = 3;
    constexpr int NOPATH = 4;
}

#endif // Dubins_hpp