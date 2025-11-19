#ifndef PathData_hpp
#define PathData_hpp

#include <vector>
#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "ObstacleManager.hpp"

namespace YAML {
template<>
struct convert<Point> {
    static Node encode(Point const& rhs) {
        Node node;
        node["x"] = rhs.x();
        node["y"] = rhs.y();
        return node;
    }

    static bool decode(Node const& node, Point& rhs) {
        if (!node.IsMap() || !node["x"] || !node["y"]) {
            return false;
        }
        rhs.x(node["x"].as<cord>());
        rhs.y(node["y"].as<cord>());
        return true;
    }
};
}

class PathData {
public:
    enum class TaskType {
        PATHFINDING = 1,
        CUTFINDING
    };
    PathData();
    PathData(std::string const& yamlFilePath);
    
    bool loadFromYAML(std::string const& filePath);
    
    std::vector<Point> const& getDestinations() const;
    ObstacleManager const& getObstacleManager() const;
    double getRadius() const;
    double getSpeed() const;
    double getWorkWidth() const;
    TaskType getTaskType() const;
    Point getTopLeft() const;
    Point getBottomRight() const;
    
    void setDestinations(std::vector<Point> const& newDestinations);
    void setRadius(double newRadius);
    void setSpeed(double newSpeed);
    void setWorkWidth(double newWorkWidth);
    void setTaskType(TaskType newTaskType);
    
    void addDestination(Point const& point);
    void clearDestinations();
    size_t getDestinationsCount() const;
    
    bool updated = false;

private:
    void update(cord x, cord y);
    void update(Point p);
    
    std::vector<Point> destinations;
    ObstacleManager obstacleManager;
    
    cord radius;
    double speed;
    cord workWidth;
    TaskType taskType;
    
    Point highCords = Point(std::numeric_limits<cord>::lowest(), std::numeric_limits<cord>::lowest());
    Point lowerCords = Point(std::numeric_limits<cord>::max(), std::numeric_limits<cord>::max());
};

#endif /* PathData_hpp */