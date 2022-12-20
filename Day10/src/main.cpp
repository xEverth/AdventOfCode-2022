#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <charconv>

template <int N>
constexpr auto FILE_PATH()
{
    return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

struct ClockCycle
{
    short clock_cycle = 0;
    short X = 1;
    short* newreg = nullptr;
    short signal_strength = 0; //result1;
    short timer = 0;
    void bind(short& ext_reg) { newreg = &ext_reg; }
    ClockCycle& operator++() {
        if (!timer--)
            X += *newreg;
        if (clock_cycle < 240)
            Scan();
        ++clock_cycle;
        if ((clock_cycle % 40) == 20)
            signal_strength += ((clock_cycle) * X);
        return *this;
    }
    ClockCycle& operator+= (int i) {
        timer = i;
        return ++ *this;
    }
    /*ClockCycle& operator++(int i) {
        timer = i;
        return ++ *this;
    }*/
    void Scan() {
        if ((clock_cycle % 40) == 0)
            std::cout << std::endl;
        if (clock_cycle % 40 == X || clock_cycle % 40 == (X + 1) || clock_cycle % 40 == (X - 1))
            std::cout << "\033[1;103m" << "#" << "\033[0m";
        else
            std::cout << ".";
    }
};

int main(int argc, char** argv)
{
    std::string buf(16, '\0');
    std::ifstream input(FILE_PATH<10>(), std::ifstream::in);
    ClockCycle clock_cycle;
    short new_reg = 0;
    clock_cycle.bind(new_reg);
    int result1 = 0;
    while (std::getline(input, buf))
    {
        ++clock_cycle;
        
        std::string_view sv(buf);
        if (sv.starts_with("noop"))
        {
            continue;
            //std::cout << "NOOP" << std::endl;
        }
        else
        {
            sv.remove_prefix(5);
            auto conv_result = std::from_chars(sv.data(), sv.data() + sv.size(), new_reg);
            clock_cycle += 1; //increment after one cycle;
            //std::cout << new_reg << std::endl;
            if (conv_result.ec == std::errc::invalid_argument)
                return 1;
                
              /*  noop
                addx 3
                addx - 5
                Execution of this program proceeds as follows :

            At the start of the first cycle, the noop instruction begins execution.During the first cycle, X is 1. After the first cycle, the noop instruction finishes execution, doing nothing.
                At the start of the second cycle, the addx 3 instruction begins execution.During the second cycle, X is still 1.
                During the third cycle, X is still 1. After the third cycle, the addx 3 instruction finishes execution, setting X to 4.
                At the start of the fourth cycle, the addx - 5 instruction begins execution.During the fourth cycle, X is still 4.
                During the fifth cycle, X is still 4. After the fifth cycle, the addx - 5 instruction finishes execution, setting X to - 1.*/
            //registers.emplace_back(curr_reg); //wasted cycle
            //registers.emplace_back(curr_reg);
        }
    }
    ++clock_cycle;
    std::cout << "Signal strenght: " << clock_cycle.signal_strength << std::endl;
}