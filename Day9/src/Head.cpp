#include "Head.h"

Head::Head(int a, int b) noexcept : Point(a, b) {};

Head::~Head() {
    tail.release();
}

void Head::MovePoint(const char& dir, int times) {
    while (times--)
        MovePoint(dir,true);
}

void Head::MovePoint(const char& dir, bool notify) {
    Point::MovePoint(dir);
    if (notify)
        NotifyMovement(this);
}

void Head::NotifyMovement(const Point* newpos) {
    tail->FollowMovement(newpos);
}

void Head::Attach(std::unique_ptr<Point> uptr) {
    tail = std::move(uptr); //release()?
}

void Head::Attach(Point* rawptr) {
    tail = std::unique_ptr<Point>(rawptr);
}

void Head::FollowMovement(const Point* newheadpos) {
    int xdiff = newheadpos->x - x, ydiff = newheadpos->y - y;
    //std::cout << std::boolalpha << (std::abs(xdiff) == 2) << '\t' << (std::abs(ydiff) == 2) << std::endl;
    if ((std::abs(xdiff) == 2) or (std::abs(ydiff) == 2))
    {
        //move in the same position if Head is 2 ahead of Tail in a specific direction
        if (xdiff > 0)
            MovePoint('R');
        if (xdiff < 0)
            MovePoint('L');
        if (ydiff > 0)
            MovePoint('U');
        if (ydiff < 0)
            MovePoint('D');
        NotifyMovement(this);
        //PrintPos();
    }
}
