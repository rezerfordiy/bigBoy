#include <algorithm>

#include "PathFindingTask.hpp"
#include "CarPathPlanner.hpp"

bool PathFindingTask::process(const PathData& data) {
    clear();
    planMap[PathFindingTask::Type::CAR] = new CarPathPlanner(data);
    planMap[PathFindingTask::Type::DRONE] = new DronePathPlanner(data);

}

void PathFindingTask::clear() {
    for (auto& pair : planMap) { 
        delete pair.second;
        pair.second = nullptr; 
    }
    planMap.clear();
}



std::vector<Point> PathFindingTask::getPointsByType(Type t) const {
    auto it = planMap.find(t);
    if (it != planMap.end() && it->second) {
        return it->second->getPath(); 
    }
    return {};  
}


PathPlanner const* PathFindingTask::operator[](Type t) const {
    auto it = planMap.find(t);
    if (it == planMap.end() || it->second == nullptr) {
        return nullptr; 
    }
    return it->second;
}