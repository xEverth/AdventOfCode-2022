#include <fstream>
#include <iostream>
#include <string>
#include "Point.h"
#include "Head.h"
#include "Tail.h"

#define TIMER 0
#if TIMER == 1
#include <chrono>
#include <thread>

struct Timer 
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << "Main took: " << ms << "ms " << std::endl;
    }
};
#endif

template <int N>
constexpr auto FILE_PATH()
{
    return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

int main(int argc, char** argv)
{
#if TIMER == 1
    Timer timer;
#endif
    constexpr int x_0 = 0, y_0 = 0;
    std::ifstream input(FILE_PATH<9>(), std::ifstream::in);
    char read_dir = '\0';
    int nmov = 0;
    constexpr size_t rope_length = 9; // 1 for part 1 9 for part 2
    Head rope_heads[rope_length];
    for (size_t i = 1; i < rope_length; ++i)
    {
        rope_heads[i - 1].Attach(&rope_heads[i]);
    }
    Tail rope_tail(0, 0);
    rope_heads[rope_length-1].Attach(&rope_tail);
    //auto comp = [](const Point& a, const Point& b) -> bool {return a < b;};
    //auto set = std::set<Point, decltype(comp)>(comp);
    while (input >> read_dir >> nmov)
    {
        //std::cout << read_dir << nmov << std::endl;
        rope_heads[0].MovePoint(read_dir, nmov);
    }
    std::cout << "Number of Unique positions of the Tail: " << rope_tail.CountUniquePos() << std::endl;
}