#pragma once

#include <fstream>
#include <iostream>
#include <string>

template <int N>
constexpr auto FILE_PATH(std::string name = "input") // "test" or "input"
{
    return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + name + ".txt";
}

template <typename Arg, typename... Args>
std::ostream& sum_str(std::ostream& os = std::cout, Arg&& arg = "", Args&&... args)
{
    os << std::forward<Arg>(arg);
    return ((os << ',' << std::forward<Args>(args)), ...);
}

template <int Bg, int Fg>
constexpr auto COLOR_TEXT(std::string colored_text = "COLORED")
{
    return std::string("\033") + "[;" + std::to_string(30 + Bg) + ";" + std::to_string(40 + Fg) + "m" + colored_text + "\033" + "[0m";
    //  "\033[{FORMAT_ATTRIBUTE};{BG_COLOR};{FG_COLOR}m{TEXT}\033[{RESET_FORMAT_ATTRIBUTE}m"
}

template <int Bg, int Fg>
constexpr auto COLOR_TEXT(int n)
{
    return COLOR_TEXT<Bg, Fg>(std::to_string(n));
}