#pragma once
#include "Point.h"
#include <cmath>

#define USE_SET 1

#if USE_VEC == 1
#include <vector>
#endif

#if USE_SET == 1
#include <unordered_set>
namespace std {
    template<>
    struct std::hash<Point>
    {
        std::size_t operator()(Point const& p) const noexcept
        {
            unsigned long n = 0;
            return n ^ (p.x << 2 * sizeof(int) | (p.y << sizeof(int)));
        }
    };
}
#endif

struct Tail : public Point {
#if USE_VEC == 1
    std::vector<Point> pastpos;
#endif
#if USE_SET == 1
    std::unordered_set<Point> pastpos;
#endif
    Tail(int a, int b) noexcept;
    void FollowMovement(const Point* newheadpos) override;
    size_t CountUniquePos() const;
    void PrintPos() const override {
        std::cout << "Tail pos X: " << x << "\t pos Y: " << y << std::endl;
    }
};
