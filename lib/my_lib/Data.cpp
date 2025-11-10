#include "Data.hpp"

Data::Data() : radius(0.0), speed(0.0), workWidth(0.0), taskType(0) {
}

Data::Data(const std::string& yamlFilePath) : radius(0.0), speed(0.0), workWidth(0.0), taskType(0) {
    loadFromYAML(yamlFilePath);
}

bool Data::loadFromYAML(const std::string& filePath) {
    try {
        YAML::Node config = YAML::LoadFile(filePath);
        
        if (config["car"]) {
            YAML::Node carNode = config["car"];
            if (carNode["radius"]) radius = carNode["radius"].as<cord>();
            if (carNode["speed"]) speed = carNode["speed"].as<double>();
            if (carNode["work_width"]) workWidth = carNode["work_width"].as<cord>();
        }
        
        if (config["task"]) {
            YAML::Node taskNode = config["task"];
            if (taskNode["type"]) taskType = taskNode["type"].as<int>();
            
            if (taskNode["obstacles"]) {
                obstacleManager.clear();
                for (const auto& pairNode : taskNode["obstacles"]) {
                    Point p1(pairNode[0]["x"].as<cord>(), pairNode[0]["y"].as<cord>());
                    Point p2(pairNode[1]["x"].as<cord>(), pairNode[1]["y"].as<cord>());
                    obstacleManager.add(p1.x(), p1.y(), p2.x(), p2.y());
                    update(p1);
                    update(p2);
                }
            }
            
            if (taskNode["destinations"]) {
                destinations.clear();
                for (const auto& pointNode : taskNode["destinations"]) {
                    Point p(pointNode["x"].as<cord>(), pointNode["y"].as<cord>());
                    destinations.push_back(p);
                    update(p);
                }
            }
        }
        
        return true;
    } catch (const YAML::Exception& e) {
        std::cerr << "YAML mistake: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error loading YAML: " << e.what() << std::endl;
        return false;
    }
}

const std::vector<Point>& Data::getDestinations() const {
    return destinations;
}

const ObstacleManager& Data::getObstacleManager() const {
    return obstacleManager;
}

double Data::getRadius() const {
    return radius;
}

double Data::getSpeed() const {
    return speed;
}

double Data::getWorkWidth() const {
    return workWidth;
}

int Data::getTaskType() const {
    return taskType;
}

Point Data::getTopLeft() const {
    return lowerCords;
}
Point Data::getBottomRight() const {
    return highCords;
}

void Data::setDestinations(const std::vector<Point>& newDestinations) {
    destinations = newDestinations;
}

void Data::setRadius(double newRadius) {
    radius = newRadius;
}

void Data::setSpeed(double newSpeed) {
    speed = newSpeed;
}

void Data::setWorkWidth(double newWorkWidth) {
    workWidth = newWorkWidth;
}

void Data::setTaskType(int newTaskType) {
    taskType = newTaskType;
}

void Data::addDestination(const Point& point) {
    destinations.push_back(point);
}

void Data::clearDestinations() {
    destinations.clear();
}

size_t Data::getDestinationsCount() const {
    return destinations.size();
}


void Data::update(cord x, cord y) {
    updated = true;
    highCords.x(fmax(highCords.x(), x));
    highCords.y(fmax(highCords.y(), y));
    
    lowerCords.x(fmin(lowerCords.x(), x));
    lowerCords.y(fmin(lowerCords.y(), y));
}

void Data::update(Point p) {
    update(p.x(), p.y());
}
