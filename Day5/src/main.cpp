#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


template <int N>
constexpr auto FILE_PATH()
{
	return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

static constexpr auto ALPHABET{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
static constexpr auto DIGIT{ "0123456789" };

int main(int argc, char** argv)
{
	std::vector<std::string> crates(9);
	std::ifstream input(FILE_PATH<5>(), std::istream::in);
	std::string line;
	while (std::getline(input,line) && input.peek() != '\n')
	{
		//std::cout << line << std::endl;
		auto pos = line.find_first_of(ALPHABET);
		for (auto pos = line.find_first_of(ALPHABET); pos != std::string::npos; pos = line.find_first_of(ALPHABET,pos+1))
		{
			//std::cout << pos << std::endl;
			crates[(pos/4)].push_back(line[pos]); //may end up with strings in opposite order
		}
	}
	/*for (auto s : crates)
		std::cout << s << std::endl;*/
	std::getline(input, line); //eat the newline
	auto stackcrates = crates;
	while (std::getline(input, line))
	{
		int nCrates, from, to;
		size_t pos = 0, endpos = 0;
		pos = line.find_first_of(DIGIT, endpos);
		endpos = line.find_first_not_of(DIGIT, pos);
		nCrates = std::stoi(line.substr(pos, endpos));
		

		pos = line.find_first_of(DIGIT, endpos);
		endpos = line.find_first_not_of(DIGIT, pos);
		from = std::stoi(line.substr(pos, endpos))-1;

		pos = line.find_first_of(DIGIT, endpos);
		endpos = line.find_first_not_of(DIGIT, pos);
		to = std::stoi(line.substr(pos, endpos))-1;
		//std::cout << nCrates << '\t' << from+1 << '\t' << to+1 << std::endl;
		//std::cout << '\n' << crates[from] << '\n' << crates[to] << '\n';
		crates[to].insert(crates[to].begin(), crates[from].rbegin()+crates[from].length()-nCrates, crates[from].rend());// PART 1
		crates[from].erase(0, nCrates);
		stackcrates[to].insert(stackcrates[to].begin(), stackcrates[from].begin(), stackcrates[from].begin()+nCrates);// PART 2
		stackcrates[from].erase(0, nCrates);
		//std::cout << crates[from] << '\t' << crates[to] << std::endl;
	}
	std::cout << "PART 1 RESULT:\n";
	for (auto& c : crates)
		std::cout << c.front();
	std::cout << std::endl << "PART 2 RESULT:\n";
	for (auto& s : stackcrates)
			std::cout << s.front();
	std::cout << std::endl;
}