#include "Tail.h"
//#include <algorithm>

void Tail::FollowMovement(const Point* newheadpos) {
    int xdiff = newheadpos->x - x, ydiff = newheadpos->y - y;
    //std::cout << std::boolalpha << (std::abs(xdiff) == 2) << '\t' << (std::abs(ydiff) == 2) << std::endl;
    if ((std::abs(xdiff) == 2) or (std::abs(ydiff) == 2))
    {
        if (xdiff > 0)
            MovePoint('R');
        if (xdiff < 0)
            MovePoint('L');
        if (ydiff > 0)
            MovePoint('U');
        if (ydiff < 0)
            MovePoint('D');
#if USE_VEC == 1
        if (std::find(pastpos.cbegin(), pastpos.cend(), Point(x, y)) == pastpos.cend())
        {
            pastpos.emplace_back(x,y); //VECTOR
        }
#endif
#if USE_SET == 1
            pastpos.emplace(x, y);
#endif
        //PrintPos();
    }
/*
*
 .....    .....    .....
.TH.. -> .T.H. -> ..TH.
.....    .....    .....

...    ...    ...
.T.    .T.    ...
.H. -> ... -> .T.
...    .H.    .H.
...    ...    ...

.....    .....    .....
.....    ..H..    ..H..
..H.. -> ..... -> ..T..
.T...    .T...    .....
.....    .....    .....

.....    .....    .....
.....    .....    .....
..H.. -> ...H. -> ..TH.
.T...    .T...    .....
.....    .....    .....

                        */
}

Tail::Tail(int a, int b) noexcept : Point(a, b) {
#if USE_VEC == 1
    pastpos.emplace_back(a,b); //VECTOR
#endif
#if USE_SET == 1
    pastpos.emplace(a,b);
#endif
}

size_t Tail::CountUniquePos() const {
    return pastpos.size(); //VECTOR
    /*
    int unique_cnt = 0;
    for (auto each = pastpos.cbegin(); each != pastpos.cend(); each = pastpos.upper_bound(*each))
    {
        std::cout << "UNIQUE: ";
        each->PrintPos();
        ++unique_cnt;

    }*/
    /*for (auto each = pastpos.cbegin(); each != pastpos.cend(); each = pastpos.equal_range(*each).second)
    {
        std::cout << "UNIQUE: ";
        each->PrintPos();
        ++unique_cnt;
    }
    return unique_cnt;*/
}