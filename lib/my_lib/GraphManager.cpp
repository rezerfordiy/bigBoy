#include "GraphManager.hpp"
#include "Data.hpp"

GraphManager::GraphManager() {}


int GraphManager::build(Data const* data) {
    std::vector<Point> points;
    for (auto const& ob : data->getObstacleManager().get()) {
        auto pnts = ob.getPoints();
        points.insert(points.end(), pnts.begin(), pnts.end());
    }
    
    if (points.empty() || !data->updated) { return 1; }
    
    
    auto fakeOb = Obstacle(data->getTopLeft().x() - 50, data->getTopLeft().y() - 50, data->getBottomRight().x() + 50, data->getBottomRight().y() + 50);
    auto fakePoints = fakeOb.getPoints();
    points.insert(points.end(), fakePoints.begin(), fakePoints.end());
    
    try {
        VoronoiDiagram vd;
        boost::polygon::construct_voronoi(points.begin(), points.end(), &vd);
        graph.buildFromVoronoi(vd, data);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при построении диаграммы";
        return 1;
    }
    
    return 0;
}
