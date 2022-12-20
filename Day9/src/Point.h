#pragma once

#include <iostream>
struct Point {
    int x, y;
    void MovePoint(const char& dir) {
        x += (dir == 'R') ? 1 : (dir == 'L') ? -1 : 0;
        y += (dir == 'U') ? 1 : (dir == 'D') ? -1 : 0;
    }
    Point(int a, int b) : x(a), y(b) {};
    /*bool operator<(const Point& other) const {
        return (x * x + y * y) < (other.x * other.x + other.y * other.y);
    }*/
    bool operator== (const Point& other) const {
        return (x == other.x) && (y == other.y);
    }
    virtual ~Point() = default;
    virtual void PrintPos() const {
        std::cout << "Point pos X: " << x << "\t pos Y: " << y << std::endl;
    }
    /*virtual void NotifyMovement(const char& dir, const Point* newpos) = 0;*/
    virtual void FollowMovement(const Point* newheadpos) {};
};