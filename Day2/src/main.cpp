#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
/*
A: Rock
B: Paper
C: Scissors

X: Rock
Y: Paper
Z: Scissors

Shift MOD 3 so that
0: ROCK
1: PAPER
C: SCISSORS

If enemy plays N, I must play (N + 1)%3 + 'X' in order to win
LOSE = 3*0 pts, DRAW = 3*1 pts, WIN = 3*2 pts
Score depending on what I play is M-'X'+1
Best possible score is 6+ (N-'A'+1)%3 +1?

IT DIDN'T QUITE WORK LIKE THAT! NEW RULES!
X means "LOSE" (0 pts), Y means "DRAW" (3 pts), Z means "WIN" (6pts)
It's similar to the BEST outcome, but now instead of always getting 6 points I'm getting 3*(M-'X')
If I need to LOSE (X), I need to choose 'X'+(N - 'A' -1) %3 
If I need to DRAW (Y), I need to choose 'X'+(N - 'A' +0) %3   // 0 is "M-'Y'"
If I need to WIN  (Z), I need to choose 'X'+(N - 'A' +1) %3 
Score would be (M - 'X')*3 + 1 + ( N - 'A' + (M-'Y') ) %3

NOTE: Since -1 % 3 is implementation defined, I added 3 inside the parenthesis so that it returns +2 as intended (If WIN: A -> C gives 3 points)

*/

struct Match {
	char result[2];
	int strat1 = 0;
	int best = 0;
	int strat2 = 0;
};


std::istream& operator>>(std::istream& stream,struct Match& in)
{
	stream >> in.result[0] >> in.result[1];
	in.strat1 = ((in.result[1] - in.result[0] - 1) % 3) * 3 + (in.result[1] - 'X'+1);
	in.best = 6 + 1 + (in.result[0] - 'A' + 1) % 3;
	in.strat2 = (in.result[1] - 'X') * 3 + 1 + ((3 + in.result[0] - 'A' + in.result[1] - 'Y') % 3);
	std::cout << in.result[0] << " " << in.result[1] << "\nSTRAT1: " << in.strat1 << "\tBEST: " << in.best << "\tSTRAT2: " << in.strat2 << std::endl;
	return stream;
}

template <int N>
constexpr auto FILE_PATH()
{
	return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

int main(int argc, char** argv)
{
	std::ifstream input(FILE_PATH<2>(), std::ifstream::in);
	if (!input.is_open()) return 1;
	std::vector<Match> results;
	std::istream_iterator<Match> iter_start(input), iter_end;
	while (iter_start != iter_end)
	{
		results.push_back(*iter_start);
		++iter_start;
	}
	std::pair<int, int> finalScore = std::accumulate(results.begin(), results.end(),
		std::make_pair<int, int>(0, 0),
		[](std::pair<int,int>& rT, Match& m) {return std::make_pair<int, int>(rT.first + m.strat1, rT.second + m.strat2); });
	//std::pair<int, int>& rT 	Errore	C2664	'std::pair<int,int> main::<lambda_1>::operator ()(std::pair<int,int> &,Match &) const':
	//impossibile convertire l'argomento 1 da 'std::pair < int, int>' a 'std::pair<int, int> &'
	//Compila con C++14 e C++17, non compila con C++20
	std::cout << "Score according to the strategy: " << finalScore.first <<"\nStrat2 score: " << finalScore.second << std::endl;
}