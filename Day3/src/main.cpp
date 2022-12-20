#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>

template <int N>
constexpr auto FILE_PATH() 
{
	return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

int priority(char a)
{
	std::cout << a << '\t' << static_cast<int>(a) << '\t' << static_cast<int>((a >= 'a') ? (a - 'a' + 1) : (a - 'A' + 26 + 1)) << std::endl;
	return static_cast<int>( ( a>='a') ? (a - 'a' + 1) : (a - 'A' + 26 + 1) );
}

int main(int argc, char** argv)
{
	std::ifstream input(FILE_PATH<3>(), std::istream::in);
	std::istream_iterator<std::string> iter_start(input), iter_end;
	int sumDuplicates = std::accumulate(std::move(iter_start),
					iter_end,
					0,
					[&](auto& result, auto& line)
					{
						auto line_half = std::next(line.begin(), std::distance(line.begin(), line.end()) / 2);
						auto duplicate = std::find_if(	line.begin(),
														line_half,
														[&line_half, &line](const char& ch) {
															return std::find(line_half, line.end(), ch) != line.end();
														});
						return result + priority(*duplicate);
					});
	std::cout << "The sum of the priorities for the duplicated items in each elf's inventory is: " << sumDuplicates << std::endl;
	input.close();
	//once data from the input stream is consumed, I have to reinstance an iterator from the input file. Input iterators are single-pass
	while (input);
	std::ifstream inputG(FILE_PATH<3>(), std::istream::in);
	std::istream_iterator<std::string> iterG_start(inputG), iterG_end;
	int badgeGroups = 0;
	while(iterG_start != iterG_end)
	{
		std::string elfGroup[3];
		std::copy_n(iterG_start, (size_t)3, elfGroup);
		for (auto& str : elfGroup)
			std::cout << str << std::endl;
		auto res = std::find_if(elfGroup[0].cbegin(),
					elfGroup[0].cend(),
					[&](const char& ch) -> bool
					{
						if (elfGroup[1].find(ch) != std::string::npos)
						{
							return (elfGroup[2].find(ch) != std::string::npos);
						} else
							return false;
					});
		if (res == std::end(elfGroup[0]))
			std::cout << "ERROR: badge not found. We got a spy" << std::endl;
		std::cout << *res << std::endl;
		badgeGroups += priority(*res);
		std::advance(iterG_start, (size_t)1);
	}

	std::cout << "The sum of the priorities for the security badges in the elves groups' inventory is: " << badgeGroups << std::endl;


}