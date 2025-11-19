#include "Obstacle.hpp"

cord Obstacle::dt = 20;

Obstacle::Obstacle(cord _x1, cord _y1, cord _x2, cord _y2) 
    : x1(std::min(_x1, _x2)), y1(std::min(_y1, _y2)),
      x2(std::max(_x1, _x2)), y2(std::max(_y1, _y2)) {}

std::vector<Point> Obstacle::getPoints() const {
    std::vector<Point> ans;
        
    ans.emplace_back(x1, y1);
    ans.emplace_back(x2, y1);
    ans.emplace_back(x2, y2);
    ans.emplace_back(x1, y2);
    
    if (dt > 0) {
        for (cord x = x1 + dt; x < x2; x += dt) {
            ans.emplace_back(x, y1);
        }
        for (cord y = y1 + dt; y < y2; y += dt) {
            ans.emplace_back(x2, y);
        }
        for (cord x = x2 - dt; x > x1; x -= dt) {
            ans.emplace_back(x, y2);
        }
        for (cord y = y2 - dt; y > y1; y -= dt) {
            ans.emplace_back(x1, y);
        }
    }
    
    std::sort(ans.begin(), ans.end());
    ans.erase(std::unique(ans.begin(), ans.end()), ans.end());
    
    return ans;
}

bool Obstacle::doesSegmentIntersect(cord x1, cord y1, cord x2, cord y2) const {
    if (isPointInside(x1, y1) || isPointInside(x2, y2)) {
        return true;
    }
    
    return checkLineSegmentIntersection(x1, y1, x2, y2, this->x1, this->y1, this->x2, this->y1) ||
           checkLineSegmentIntersection(x1, y1, x2, y2, this->x2, this->y1, this->x2, this->y2) ||
           checkLineSegmentIntersection(x1, y1, x2, y2, this->x2, this->y2, this->x1, this->y2) ||
           checkLineSegmentIntersection(x1, y1, x2, y2, this->x1, this->y2, this->x1, this->y1);
}

bool Obstacle::isPointInside(cord x, cord y) const {
    return x >= x1 && x <= x2 && y >= y1 && y <= y2;
}

bool Obstacle::checkLineSegmentIntersection(cord p0_x, cord p0_y, cord p1_x, cord p1_y,
                                 cord p2_x, cord p2_y, cord p3_x, cord p3_y) const {
    cord s1_x = p1_x - p0_x;
    cord s1_y = p1_y - p0_y;
    cord s2_x = p3_x - p2_x;
    cord s2_y = p3_y - p2_y;

    cord s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    cord t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    return s >= 0 && s <= 1 && t >= 0 && t <= 1;
}