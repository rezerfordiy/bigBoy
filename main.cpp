#include <iostream>
#include <string>
#include <cstring>

#include <QApplication>

#include "Mainwindow.hpp"
#include "Solver.hpp"


constexpr int AMOUNT = 100;

void printObstacles(const ObstacleManager& obstacleManager) {
    for (auto const& ob : obstacleManager.get()) {
        std::cout << "OBSTACLE: " << ob.x1 << ", " << ob.y1 << ", " 
                  << ob.x2 - ob.x1 << ", " << ob.y2 - ob.y1 << std::endl;
    }
}

void printGraphNodes(const Graph& graph) {
    for (const auto& node : graph.nodes) {
        std::cout << "GRAPH_NODE: " << node.x << ", " << node.y << std::endl;
    }
}

void printGraphEdges(const Graph& graph) {
    for (const auto& edge : graph.edges) {
        cord x1 = graph.nodes[edge.from].x;
        cord y1 = graph.nodes[edge.from].y;
        cord x2 = graph.nodes[edge.to].x;
        cord y2 = graph.nodes[edge.to].y;
        std::cout << "GRAPH_EDGE: " << x1 << ", " << y1 << " -> " << x2 << ", " << y2 << std::endl;
    }
}

std::vector<Point> sampleDubinsPath(const std::vector<DubinsPath>& dubinsPaths) {
    std::vector<Point> points;
    
    for (const auto& dubin : dubinsPaths) {
        cord length = dubin.length();
        if (length <= 0) continue;
        
        for (int j = 0; j < AMOUNT; j++) {
            cord t = (length * j) / AMOUNT;      
            DubinsConfiguration conf = dubin.sample(t);
            points.emplace_back(conf.x, conf.y);
        }
    }
    
    return points;
}

void printPathPoints(const std::vector<Point>& points) {
    for (const auto& point : points) {
        std::cout << "PATH_POINT: " << point.x() << ", " << point.y() << std::endl;
    }
}

void printHorizontalCoverage(const Point& p1, const Point& p2, cord workWidth) {
    cord leftX = std::min(p1.x(), p2.x());
    cord rightX = std::max(p1.x(), p2.x());
    cord y = p1.y();
    cord rectY = y - workWidth / 2;
    
    std::cout << "COVERAGE_RECT: " << leftX << ", " << rectY << ", " 
              << rightX - leftX << ", " << workWidth << std::endl;
}

void printVerticalCoverage(const Point& p1, const Point& p2, cord workWidth) {
    cord topY = std::min(p1.y(), p2.y());
    cord bottomY = std::max(p1.y(), p2.y());
    cord x = p1.x();
    cord rectX = x - workWidth / 2;
    
    std::cout << "COVERAGE_RECT: " << rectX << ", " << topY << ", " 
              << workWidth << ", " << bottomY - topY << std::endl;
}

void processTask1(Solver& slv) {
    auto& task1 = slv.getPathFindingTask();
    auto& obstacleManager = task1.getData().getObstacleManager();
    auto& graph = task1.getGraphManager().graph;
    const auto& dubinsPaths = task1.getPathDubins();
    
    printObstacles(obstacleManager);
    printGraphNodes(graph);
    printGraphEdges(graph);
    
    auto pathPoints = sampleDubinsPath(dubinsPaths);
    printPathPoints(pathPoints);
}

void processTask2(Solver& slv) {
    auto segments = slv.getTask2().getPathPoint();
    cord workWidth = slv.getData().getWorkWidth();
    
    for (const auto& segment : segments) {
        Point p1 = segment.first;
        Point p2 = segment.second;
        
        std::cout << "SEGMENT: " << p1.x() << ", " << p1.y() << " -> " 
                  << p2.x() << ", " << p2.y() << std::endl;
        
        if (p1.y() == p2.y()) {
            printHorizontalCoverage(p1, p2, workWidth);
        } else if (p1.x() == p2.x()) {
            printVerticalCoverage(p1, p2, workWidth);
        }
    }
}

int runCoreMode(const std::string& filePath) {
    Solver slv(filePath);
    PathData::TaskType type = slv.getData().getTaskType();
    
    switch (type) {
        case PathData::TaskType::PATHFINDING:
            processTask1(slv);
            break;
        case PathData::TaskType::CUTFINDING:
            processTask2(slv);
            break;
        default:
            std::cerr << "Data error: unknown type " << (int)type << std::endl;
            return 1;
    }
    
    return 0;
}

int runGuiMode(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    
    return app.exec();
}


int main(int argc, char *argv[]) {
    if (argc >= 3 && strcmp(argv[1], "core") == 0) {
        return runCoreMode(argv[2]);
    }
    
    return runGuiMode(argc, argv);
}
