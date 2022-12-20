#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

template <int N>
constexpr auto FILE_PATH()
{
	return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

int main(int argc, char** argv)
{
	std::ifstream input(FILE_PATH<1>(), std::ifstream::in);
	if (!input.is_open()) return 1;
	std::vector<uint32_t> partialSum;
	partialSum.reserve(10);
	uint32_t elfSum = 0;
	char c;
	while (input)
	{
		std::istream_iterator<uint32_t> iter_start(input), iter_end;
		if (iter_start != iter_end)
		{
			elfSum += *iter_start;
		}
		input.get(c);
		if (c != EOF && input.peek() == '\n')
		{
			partialSum.push_back(elfSum);
			elfSum = 0;
		}
		//std::cout << "elf calories: " << elfSum << std::endl;
	}
	/*auto pos_max_element = std::max_element(partialSum.begin(), partialSum.end());
	std::cout << "Numero più alto: " << *pos_max_element << std::endl;
	*/
	std::ranges::sort(partialSum, std::ranges::greater());
	std::cout << "Numero di calorie più alto: " << partialSum.at(0) << std::endl;
	std::cout << "Somma provviste dei tre elfi più previgenti: " << std::accumulate(partialSum.begin(), std::next(partialSum.begin(), 3), 0) << std::endl;
	return 0;
}