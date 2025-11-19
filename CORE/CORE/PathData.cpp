#include "PathData.hpp"

PathData::PathData() : radius(0.0), speed(0.0), workWidth(0.0), taskType(PathData::TaskType::CUTFINDING) {
}

PathData::PathData(std::string const& yamlFilePath) : radius(0.0), speed(0.0), workWidth(0.0), taskType(PathData::TaskType::CUTFINDING) {
    loadFromYAML(yamlFilePath);
}

bool PathData::loadFromYAML(std::string const& filePath) {
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
            if (taskNode["type"]) taskType = TaskType(taskNode["type"].as<int>());
            
            if (taskNode["obstacles"]) {
                obstacleManager.clear();
                for (auto const& pairNode : taskNode["obstacles"]) {
                    Point p1(pairNode[0]["x"].as<cord>(), pairNode[0]["y"].as<cord>());
                    Point p2(pairNode[1]["x"].as<cord>(), pairNode[1]["y"].as<cord>());
                    obstacleManager.add(p1.x(), p1.y(), p2.x(), p2.y());
                    update(p1);
                    update(p2);
                }
            }
            
            if (taskNode["destinations"]) {
                destinations.clear();
                for (auto const& pointNode : taskNode["destinations"]) {
                    Point p(pointNode["x"].as<cord>(), pointNode["y"].as<cord>());
                    destinations.push_back(p);
                    update(p);
                }
            }
        }
        
        return true;
    } catch (YAML::Exception const& e) {
        std::cerr << "YAML mistake: " << e.what() << std::endl;
        return false;
    } catch (std::exception const& e) {
        std::cerr << "Error loading YAML: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Point> const& PathData::getDestinations() const {
    return destinations;
}

ObstacleManager const& PathData::getObstacleManager() const {
    return obstacleManager;
}

double PathData::getRadius() const {
    return radius;
}

double PathData::getSpeed() const {
    return speed;
}

double PathData::getWorkWidth() const {
    return workWidth;
}

PathData::TaskType PathData::getTaskType() const {
    return taskType;
}

Point PathData::getTopLeft() const {
    return lowerCords;
}

Point PathData::getBottomRight() const {
    return highCords;
}

void PathData::setDestinations(std::vector<Point> const& newDestinations) {
    destinations = newDestinations;
}

void PathData::setRadius(double newRadius) {
    radius = newRadius;
}

void PathData::setSpeed(double newSpeed) {
    speed = newSpeed;
}

void PathData::setWorkWidth(double newWorkWidth) {
    workWidth = newWorkWidth;
}

void PathData::setTaskType(PathData::TaskType newTaskType) {
    taskType = newTaskType;
}

void PathData::addDestination(Point const& point) {
    destinations.push_back(point);
}

void PathData::clearDestinations() {
    destinations.clear();
}

size_t PathData::getDestinationsCount() const {
    return destinations.size();
}

void PathData::update(cord x, cord y) {
    updated = true;
    highCords.x(fmax(highCords.x(), x));
    highCords.y(fmax(highCords.y(), y));
    
    lowerCords.x(fmin(lowerCords.x(), x));
    lowerCords.y(fmin(lowerCords.y(), y));
}

void PathData::update(Point p) {
    update(p.x(), p.y());
}