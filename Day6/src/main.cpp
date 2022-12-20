#include <fstream>
#include <iostream>
#include <string>


template <int N>
constexpr auto FILE_PATH()
{
	return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}


int main(int argc, char** argv)
{
	std::ifstream input(FILE_PATH<6>(), std::istream::in);
	char buf[3], buf2[13], c;
	int counter = 0, start_packet = 0, start_message = 0;
	while (input.get(c))
	{
		++counter;
		if (!start_packet && counter > 3)
		{
			bool contained = buf[0]==c || buf[1]==c || buf[2]==c;
				if (!contained)
				{
					contained = buf[0] == buf[1] || buf[0] == buf[2] || buf[1] == buf[2];
					if (!contained)
					{
						start_packet = counter;
					}
				}
		}
		if (!start_message && counter > 13)
		{
			bool contained = false;
			for (size_t i = 0; i < 13; ++i)
			{
				contained |= buf2[i] == c;
				for (size_t j = i + 1; j < 13; ++j)
				{
					contained |= buf2[i] == buf2[j];
				}
			}
			if (!contained)
			{
				start_message = counter;
			}
		}

		buf[counter % 3] = c;
		buf2[counter % 13] = c;
	}
	std::cout << "Packet starts at: " << start_packet << '\n'
	<< "Message starts at: " << start_message << std::endl;
}