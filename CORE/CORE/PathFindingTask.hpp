#ifndef PathFindingTask_hpp
#define PathFindingTask_hpp

#include <unordered_map>
#include <vector>

#include "PathData.hpp"

class PathPlanner;

class PathFindingTask {
public:
    enum class Type {
        CAR,
        DRONE
    };

private:
    struct TypeHash {
        size_t operator()(Type t) const {
            return static_cast<size_t>(t);
        }
    };
    
public:
    PathFindingTask() = default;
    
    PathFindingTask(const PathFindingTask& other) = delete;
    PathFindingTask(PathFindingTask&& other) = delete;
    PathFindingTask& operator=(const PathFindingTask& other) = delete;
    PathFindingTask& operator=(PathFindingTask&& other) = delete;
    
    ~PathFindingTask() {
        clear();  
    }
    
    bool process(const PathData& data);
    std::vector<Point> getPointsByType(Type t) const;
    PathPlanner const* operator[](Type t) const;
    void clear();
private:
    std::unordered_map<Type, PathPlanner*, TypeHash> planMap;
};

#endif /* PathFindingTask_hpp */