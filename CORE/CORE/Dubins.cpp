#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <cmath>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <limits>

#include "Dubins.hpp"

namespace {
    enum SegmentType {
        L_SEG = 0,
        S_SEG = 1,
        R_SEG = 2
    };

    SegmentType const DIRDATA[][3] = {
        { L_SEG, S_SEG, L_SEG },
        { L_SEG, S_SEG, R_SEG },
        { R_SEG, S_SEG, L_SEG },
        { R_SEG, S_SEG, R_SEG },
        { R_SEG, L_SEG, R_SEG },
        { L_SEG, R_SEG, L_SEG }
    };
}

DubinsPath::DubinsPath(DubinsConfiguration const& start, cord const param[3], cord rho, DubinsPathType type)
    : start(start), rho(rho), type(type) {
    this->param[0] = param[0];
    this->param[1] = param[1];
    this->param[2] = param[2];
}

cord DubinsPath::getParam(int i) const {
    if (i < 0 || i > 2) {
        throw std::out_of_range("Segment index must be between 0 and 2");
    }
    return param[i];
}

cord DubinsPath::length() const {
    return (param[0] + param[1] + param[2]) * rho;
}

cord DubinsPath::segmentLength(int i) const {
    return getParam(i) * rho;
}

cord DubinsPath::segmentLengthNormalized(int i) const {
    return getParam(i);
}

DubinsConfiguration DubinsPath::sample(cord t) const {
    return DubinsSolver::samplePath(*this, t);
}

DubinsConfiguration DubinsPath::endPoint() const {
    return sample(length() - 1e-10);
}

std::unique_ptr<DubinsPath> DubinsPath::extractSubpath(cord t) const {
    if (t < 0 || t > length()) {
        throw std::invalid_argument("Parameter t is out of range");
    }

    cord tprime = t / rho;
    cord newParams[3] = {
        std::min(param[0], tprime),
        std::min(param[1], tprime - newParams[0]),
        std::min(param[2], tprime - newParams[0] - newParams[1])
    };

    return std::make_unique<DubinsPath>(start, newParams, rho, type);
}

std::string DubinsPath::pathTypeToString(DubinsPathType type) {
    switch (type) {
        case DubinsPathType::LSL: return "LSL";
        case DubinsPathType::LSR: return "LSR";
        case DubinsPathType::RSL: return "RSL";
        case DubinsPathType::RSR: return "RSR";
        case DubinsPathType::RLR: return "RLR";
        case DubinsPathType::LRL: return "LRL";
        default: return "UNKNOWN";
    }
}

cord DubinsSolver::fmodr(cord x, cord y) {
    return x - y * floor(x / y);
}

cord DubinsSolver::mod2pi(cord theta) {
    return fmodr(theta, 2 * M_PI);
}

DubinsConfiguration DubinsSolver::samplePath(DubinsPath const& path, cord t) {
    if (t < 0 || t > path.length()) {
        throw std::invalid_argument("Parameter t is out of range");
    }

    cord tprime = t / path.getRho();
    DubinsConfiguration qi(0.0, 0.0, path.getStart().theta);
    SegmentType const* types = DIRDATA[static_cast<int>(path.getType())];
    
    cord p1 = path.getParam(0);
    cord p2 = path.getParam(1);
    
    DubinsConfiguration q1 = dubinsSegment(p1, qi, types[0]);
    DubinsConfiguration q2 = dubinsSegment(p2, q1, types[1]);
    
    DubinsConfiguration result;
    if (tprime < p1) {
        result = dubinsSegment(tprime, qi, types[0]);
    } else if (tprime < (p1 + p2)) {
        result = dubinsSegment(tprime - p1, q1, types[1]);
    } else {
        result = dubinsSegment(tprime - p1 - p2, q2, types[2]);
    }
    
    result.x = result.x * path.getRho() + path.getStart().x;
    result.y = result.y * path.getRho() + path.getStart().y;
    result.theta = mod2pi(result.theta);
    
    return result;
}

std::unique_ptr<DubinsPath> DubinsSolver::shortestPath(DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho) {
    IntermediateResults in;
    int errcode = calculateIntermediateResults(&in, q0, q1, rho);
    if (errcode != DubinsError::OK) {
        return nullptr;
    }

    cord bestCost = std::numeric_limits<cord>::infinity();
    cord bestParams[3] = {0, 0, 0};
    DubinsPathType bestType = DubinsPathType::LSL;

    for (int i = 0; i < 6; i++) {
        DubinsPathType pathType = static_cast<DubinsPathType>(i);
        cord params[3];
        
        if (dubinsWord(&in, pathType, params) == DubinsError::OK) {
            cord cost = params[0] + params[1] + params[2];
            if (cost < bestCost) {
                bestCost = cost;
                bestParams[0] = params[0];
                bestParams[1] = params[1];
                bestParams[2] = params[2];
                bestType = pathType;
            }
        }
    }

    if (bestCost == std::numeric_limits<cord>::infinity()) {
        return nullptr;
    }

    return std::make_unique<DubinsPath>(q0, bestParams, rho, bestType);
}

std::unique_ptr<DubinsPath> DubinsSolver::path(DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho, DubinsPathType pathType) {
    IntermediateResults in;
    int errcode = calculateIntermediateResults(&in, q0, q1, rho);
    if (errcode != DubinsError::OK) {
        return nullptr;
    }

    cord params[3];
    errcode = dubinsWord(&in, pathType, params);
    if (errcode != DubinsError::OK) {
        return nullptr;
    }

    return std::make_unique<DubinsPath>(q0, params, rho, pathType);
}

int DubinsSolver::sampleMany(DubinsPath const& path, cord stepSize, SamplingCallback const& callback) {
    cord x = 0.0;
    cord length = path.length();
    
    while (x < length) {
        DubinsConfiguration q = samplePath(path, x);
        int retcode = callback(q, x);
        if (retcode != 0) {
            return retcode;
        }
        x += stepSize;
    }
    return 0;
}

int DubinsSolver::calculateIntermediateResults(IntermediateResults* in, DubinsConfiguration const& q0, DubinsConfiguration const& q1, cord rho) {
    if (rho <= 0.0) {
        return DubinsError::BADRHO;
    }

    cord dx = q1.x - q0.x;
    cord dy = q1.y - q0.y;
    cord D = sqrt(dx * dx + dy * dy);
    cord d = D / rho;
    cord theta = 0;

    if (d > 0) {
        theta = mod2pi(atan2(dy, dx));
    }
    
    cord alpha = mod2pi(q0.theta - theta);
    cord beta = mod2pi(q1.theta - theta);

    in->alpha = alpha;
    in->beta = beta;
    in->d = d;
    in->sa = sin(alpha);
    in->sb = sin(beta);
    in->ca = cos(alpha);
    in->cb = cos(beta);
    in->cAb = cos(alpha - beta);
    in->dSq = d * d;

    return DubinsError::OK;
}

DubinsConfiguration DubinsSolver::dubinsSegment(cord t, DubinsConfiguration const& qi, int segmentType) {
    cord st = sin(qi.theta);
    cord ct = cos(qi.theta);
    DubinsConfiguration qt;

    if (segmentType == L_SEG) {
        qt.x = +sin(qi.theta + t) - st;
        qt.y = -cos(qi.theta + t) + ct;
        qt.theta = t;
    } else if (segmentType == R_SEG) {
        qt.x = -sin(qi.theta - t) + st;
        qt.y = +cos(qi.theta - t) - ct;
        qt.theta = -t;
    } else if (segmentType == S_SEG) {
        qt.x = ct * t;
        qt.y = st * t;
        qt.theta = 0.0;
    }

    qt.x += qi.x;
    qt.y += qi.y;
    qt.theta += qi.theta;

    return qt;
}

int DubinsSolver::dubinsLSL(IntermediateResults const* in, cord out[3]) {
    cord tmp0 = in->d + in->sa - in->sb;
    cord pSq = 2 + in->dSq - (2 * in->cAb) + (2 * in->d * (in->sa - in->sb));

    if (pSq >= 0) {
        cord tmp1 = atan2((in->cb - in->ca), tmp0);
        out[0] = mod2pi(tmp1 - in->alpha);
        out[1] = sqrt(pSq);
        out[2] = mod2pi(in->beta - tmp1);
        return DubinsError::OK;
    }
    return DubinsError::NOPATH;
}

int DubinsSolver::dubinsRSR(IntermediateResults const* in, cord out[3]) {
    cord tmp0 = in->d - in->sa + in->sb;
    cord pSq = 2 + in->dSq - (2 * in->cAb) + (2 * in->d * (in->sb - in->sa));
    
    if (pSq >= 0) {
        cord tmp1 = atan2((in->ca - in->cb), tmp0);
        out[0] = mod2pi(in->alpha - tmp1);
        out[1] = sqrt(pSq);
        out[2] = mod2pi(tmp1 - in->beta);
        return DubinsError::OK;
    }
    return DubinsError::NOPATH;
}

int DubinsSolver::dubinsLSR(IntermediateResults const* in, cord out[3]) {
    cord pSq = -2 + in->dSq + (2 * in->cAb) + (2 * in->d * (in->sa + in->sb));
    
    if (pSq >= 0) {
        cord p = sqrt(pSq);
        cord tmp0 = atan2((-in->ca - in->cb), (in->d + in->sa + in->sb)) - atan2(-2.0, p);
        out[0] = mod2pi(tmp0 - in->alpha);
        out[1] = p;
        out[2] = mod2pi(tmp0 - mod2pi(in->beta));
        return DubinsError::OK;
    }
    return DubinsError::NOPATH;
}

int DubinsSolver::dubinsRSL(IntermediateResults const* in, cord out[3]) {
    cord pSq = -2 + in->dSq + (2 * in->cAb) - (2 * in->d * (in->sa + in->sb));
    
    if (pSq >= 0) {
        cord p = sqrt(pSq);
        cord tmp0 = atan2((in->ca + in->cb), (in->d - in->sa - in->sb)) - atan2(2.0, p);
        out[0] = mod2pi(in->alpha - tmp0);
        out[1] = p;
        out[2] = mod2pi(in->beta - tmp0);
        return DubinsError::OK;
    }
    return DubinsError::NOPATH;
}

int DubinsSolver::dubinsRLR(IntermediateResults const* in, cord out[3]) {
    cord tmp0 = (6.0 - in->dSq + 2 * in->cAb + 2 * in->d * (in->sa - in->sb)) / 8.0;
    cord phi = atan2(in->ca - in->cb, in->d - in->sa + in->sb);
    
    if (fabs(tmp0) <= 1) {
        cord p = mod2pi(2 * M_PI - acos(tmp0));
        cord t = mod2pi(in->alpha - phi + mod2pi(p / 2.0));
        out[0] = t;
        out[1] = p;
        out[2] = mod2pi(in->alpha - in->beta - t + mod2pi(p));
        return DubinsError::OK;
    }
    return DubinsError::NOPATH;
}

int DubinsSolver::dubinsLRL(IntermediateResults const* in, cord out[3]) {
    cord tmp0 = (6.0 - in->dSq + 2 * in->cAb + 2 * in->d * (in->sb - in->sa)) / 8.0;
    cord phi = atan2(in->ca - in->cb, in->d + in->sa - in->sb);
    
    if (fabs(tmp0) <= 1) {
        cord p = mod2pi(2 * M_PI - acos(tmp0));
        cord t = mod2pi(-in->alpha - phi + p / 2.0);
        out[0] = t;
        out[1] = p;
        out[2] = mod2pi(mod2pi(in->beta) - in->alpha - t + mod2pi(p));
        return DubinsError::OK;
    }
    return DubinsError::NOPATH;
}

int DubinsSolver::dubinsWord(IntermediateResults const* in, DubinsPathType pathType, cord out[3]) {
    switch (pathType) {
        case DubinsPathType::LSL: return dubinsLSL(in, out);
        case DubinsPathType::RSL: return dubinsRSL(in, out);
        case DubinsPathType::LSR: return dubinsLSR(in, out);
        case DubinsPathType::RSR: return dubinsRSR(in, out);
        case DubinsPathType::LRL: return dubinsLRL(in, out);
        case DubinsPathType::RLR: return dubinsRLR(in, out);
        default: return DubinsError::NOPATH;
    }
}