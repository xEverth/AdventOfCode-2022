#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <int N>
constexpr auto FILE_PATH()
{
	return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

struct Assignment
{
	std::pair<int, int> elf1, elf2;
	bool contained = 0;
	bool overlap = 0;
	
	public:
	static int contained_cnt;
	static int overlap_cnt;
	Assignment(int a1, int a2, int a3, int a4) : elf1(std::make_pair(a1,a2)), elf2(std::make_pair(a3,a4)) 
	{
		contained = isContained(*this);
		overlap = isOverlapping(*this);

		std::cout << elf1.first << '\t' << elf1.second << std::endl
			<< elf2.first << '\t' << elf2.second << std::endl
			<< std::boolalpha << "Contained: " << contained << "\t Overlapping: " << overlap << std::endl
			<< "--------------------------------" << std::endl;

		if (contained)
			Assignment::contained_cnt += 1;
		if (overlap)
			Assignment::overlap_cnt += 1;
	};
	friend bool isContained(Assignment&);
	friend bool isOverlapping(Assignment&);
};

int Assignment::contained_cnt = 0;
int Assignment::overlap_cnt = 0;

bool isContained(Assignment& ass)
{
	std::pair<int, int>& elf1 = ass.elf1;
	std::pair<int, int>& elf2 = ass.elf2;
	return ((elf1.first < elf2.first+1) && (elf1.second > elf2.second-1) || 
			(elf1.first > elf2.first-1) && (elf1.second < elf2.second+1));
	//faster than greater_eq and less_eq
}

bool isOverlapping(Assignment& ass)
{
	std::pair<int, int>& elf1 = ass.elf1;
	std::pair<int, int>& elf2 = ass.elf2;
	return ass.contained || (elf1.first > elf2.first-1) && (elf1.first < elf2.second+1) ||
							(elf1.second > elf2.first-1) && (elf1.second < elf2.second+1) ||
							(elf2.first > elf1.first-1) && (elf2.first < elf1.second+1) ||
							(elf2.second > elf1.first-1) && (elf2.second < elf1.second+1);
}

int main(int argc, char** argv)
{
	std::ifstream input(FILE_PATH<4>(), std::istream::in);
	std::string line;
	std::vector<Assignment> vec;
	vec.reserve(100);
	while (std::getline(input, line))
	{
		size_t next_delim = 0;
		int buf[4], pos = 0;
		while (next_delim != std::string::npos)
		{
			next_delim = line.find_first_of("-,");
			int a = std::stoi(line.substr(0, next_delim));
			buf[pos++] = a;
			line.erase(0, next_delim + 1);
		}
		vec.emplace_back( Assignment(buf[0],buf[1],buf[2],buf[3]) );
	}
	std::cout	<< "Conttained shifts : "	<< Assignment::contained_cnt << std::endl
				<< "Overlapping shifts: "	<< Assignment::overlap_cnt << std::endl;
}