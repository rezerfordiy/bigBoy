#ifndef Data_hpp
#define Data_hpp

#include <vector>
#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "ObstacleManager.hpp"

namespace YAML {
template<>
struct convert<Point> {
    static Node encode(const Point& rhs) {
        Node node;
        node["x"] = rhs.x();
        node["y"] = rhs.y();
        return node;
    }

    static bool decode(const Node& node, Point& rhs) {
        if (!node.IsMap() || !node["x"] || !node["y"]) {
            return false;
        }
        rhs.x(node["x"].as<cord>());
        rhs.y(node["y"].as<cord>());
        return true;
    }
};
}

class Data {
public:
    Data();
    Data(const std::string& yamlFilePath);
    
    bool loadFromYAML(const std::string& filePath);
    
    const std::vector<Point>& getDestinations() const;
    const ObstacleManager& getObstacleManager() const;
    double getRadius() const;
    double getSpeed() const;
    double getWorkWidth() const;
    int getTaskType() const;
    Point getTopLeft() const;
    Point getBottomRight() const;
    
    
    void setDestinations(const std::vector<Point>& newDestinations);
    void setRadius(double newRadius);
    void setSpeed(double newSpeed);
    void setWorkWidth(double newWorkWidth);
    void setTaskType(int newTaskType);
    
    void addDestination(const Point& point);
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
    
    int taskType;
    
    Point highCords = Point(std::numeric_limits<cord>::lowest(), std::numeric_limits<cord>::lowest());
    Point lowerCords = Point(std::numeric_limits<cord>::max(), std::numeric_limits<cord>::max());
    

};

#endif /* Data_hpp */
