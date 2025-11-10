#include <iostream>
#include <string>
#include <cstring>
./
#include <QApplication>

#include "Mainwindow.hpp"

#include "Solver.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    if (argc >= 3 && strcmp(argv[1], "core") == 0) {
        std::string filePath = argv[2];
        Solver slv(filePath);
        int type = slv.data.getTaskType();
        
        const static int AMOUNT = 100;
        
        if (type == 1) {
            auto& obstacleManager = slv.task1.getData().getObstacleManager();
            for (auto const& ob : obstacleManager.get()) {
                std::cout << "OBSTACLE: " << ob.x1 << ", " << ob.y1 << ", " 
                          << ob.x2 - ob.x1 << ", " << ob.y2 - ob.y1 << std::endl;
            }
            
            auto& graph = slv.task1.getGraphManager().graph;
            for (const auto& node : graph.nodes) {
                std::cout << "GRAPH_NODE: " << node.x << ", " << node.y << std::endl;
            }
            for (const auto& edge : graph.edges) {
                cord x1 = graph.nodes[edge.from].x;
                cord y1 = graph.nodes[edge.from].y;
                cord x2 = graph.nodes[edge.to].x;
                cord y2 = graph.nodes[edge.to].y;
                std::cout << "GRAPH_EDGE: " << x1 << ", " << y1 << " -> " << x2 << ", " << y2 << std::endl;
            }
            
            std::vector<Point> points;
            for (int i = 0; i < slv.task1.getPathDubins().size(); i++) {
                DubinsPath dubin = slv.task1.getPathDubins()[i];
                cord length = dubin.length();
                if (length > 0) {
                    for (int j = 0; j < AMOUNT; j++) {
                        cord t = (length * j) / AMOUNT;      
                        DubinsConfiguration conf = dubin.sample(t);
                        points.emplace_back(conf.x, conf.y);
                    }
                }
            }
            for (const auto& point : points) {
                std::cout << "PATH_POINT: " << point.x() << ", " << point.y() << std::endl;
            }
            
        } else if (type == 2) {            
            auto segments = slv.task2.getPathPoint();
            cord workWidth = slv.data.getWorkWidth();
            for (const auto& segment : segments) {
                Point p1 = segment.first;
                Point p2 = segment.second;
                std::cout << "SEGMENT: " << p1.x() << ", " << p1.y() << " -> " << p2.x() << ", " << p2.y() << std::endl;
                
                if (p1.y() == p2.y()) {
                    cord leftX = std::min(p1.x(), p2.x());
                    cord rightX = std::max(p1.x(), p2.x());
                    cord y = p1.y();
                    cord rectY = y - workWidth / 2;
                    std::cout << "COVERAGE_RECT: " << leftX << ", " << rectY << ", " 
                              << rightX - leftX << ", " << workWidth << std::endl;
                }
                else if (p1.x() == p2.x()) {
                    cord topY = std::min(p1.y(), p2.y());
                    cord bottomY = std::max(p1.y(), p2.y());
                    cord x = p1.x();
                    cord rectX = x - workWidth / 2;
                    std::cout << "COVERAGE_RECT: " << rectX << ", " << topY << ", " 
                              << workWidth << ", " << bottomY - topY << std::endl;
                }
            }
        } else {
            std::cerr << "Data error: unknown type" << std::endl;
            return 1;
        }
        
        return 0;
    }
    
    MainWindow window;
    window.show();
    
    return app.exec();
}