#pragma once
#include "Point.h"
#include <memory>

struct Head : public Point {
    std::unique_ptr<Point> tail;
    Head() : Point(0, 0), tail(nullptr) {};
    Head(int a, int b) noexcept;
    ~Head() override;
    void Attach(std::unique_ptr<Point> uptr);
    void Attach(Point*);
    //void MovePoint(const char& dir) override;
    void MovePoint(const char& dir, bool notify = false);
    void MovePoint(const char& dir, int times);
    void NotifyMovement(const Point* newpos);
    void FollowMovement(const Point* newheadpos) override;
    void PrintPos() const override {
        std::cout << "Head pos X: " << x << "\t pos Y: " << y << std::endl;
    }
};