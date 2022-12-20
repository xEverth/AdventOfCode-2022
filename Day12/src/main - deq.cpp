#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <numeric>
#include <future>
#include <queue>
#include <vector>
#include <algorithm>

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
    return std::string("\033") + "[;" + std::to_string(30+Bg) + ";" + std::to_string(40+Fg) + "m" + colored_text + "\033" + "[0m";
//  "\033[{FORMAT_ATTRIBUTE};{BG_COLOR};{FG_COLOR}m{TEXT}\033[{RESET_FORMAT_ATTRIBUTE}m"
}

template <int Bg, int Fg>
constexpr auto COLOR_TEXT(int n)
{
    return COLOR_TEXT<Bg, Fg>(std::to_string(n));
}

static size_t row_length;
static size_t tot_rows;

bool is_adjacent(size_t curr_index, size_t other_index)
{
    std::cout << "Row l" << row_length << '\t' << "Tot R" << tot_rows << '\n';
    bool is_left = (curr_index - other_index + 1) == 0; //if index%row_length !== 0
    bool is_right = (curr_index - other_index - 1) == 0;
    bool is_up = (other_index - curr_index - row_length) == 0;
    bool is_down = (curr_index - other_index - row_length) == 0;
    std::cout << std::boolalpha << is_left << '\t' << is_right << '\t' << is_up << '\t' << is_down;
    return is_left || is_right || is_up || is_down;
}



uint16_t inline numeric_height(char curr_height)
{
    if (curr_height < 'a') //UPPERCASE in ASCII are lower in value than uppercase
        return (curr_height - 'E'+26); //so A > a and E > z
    else
        return (curr_height - 'a');
}

bool inline can_reach(uint16_t curr, uint16_t next)
{
    return (next - 2 - curr) < 0;
}

bool inline can_reach(char curr, char next)
{
    return can_reach(numeric_height(curr), numeric_height(next));
}

bool inline can_go_left(size_t pos)
{
    return !((pos % row_length) == 0);
}

size_t inline go_left(size_t pos)
{
    return pos - 1;
}

bool inline can_go_right(size_t pos)
{
    return !((pos % row_length) == ((row_length - 1) % row_length));
}

size_t inline go_right(size_t pos)
{
    return pos + 1;
}

bool inline can_go_up(size_t pos)
{
    return !((pos / row_length)==0);
}

size_t inline go_up(size_t pos)
{
    return pos - row_length;
}

bool inline can_go_down(size_t pos)
{
    return !((pos / row_length) == (tot_rows-1));
}

size_t inline go_down(size_t pos)
{
    return pos + row_length;
}

//enum class directions : uint8_t {
//    DIR_UP = 0,
//    DIR_DOWN = 1,
//    DIR_LEFT = 2,
//    DIR_RIGHT = 3,
//    DIR_STARTING = 4
//};
//
//constexpr std::initializer_list<directions> dir_list = {directions::DIR_UP, directions::DIR_DOWN, directions::DIR_LEFT, directions::DIR_RIGHT };
//
//std::ostream& operator<<(std::ostream& out, const directions dir) {
//    constexpr std::string_view name_dir[5] = {"DIR_UP", "DIR_DOWN", "DIR_LEFT", "DIR_RIGHT", "DIR_STARTING"};
//    return out << name_dir[std::to_underlying(dir)];
//}

bool (*tests[])(size_t) = { can_go_up, can_go_down, can_go_left, can_go_right };
size_t(*moves[])(size_t) = { go_up, go_down, go_left, go_right };
/*
* (a^b)==1 works to tell if 
     for (uint8_t a = 0; a < 4 ; ++a)
        for (uint8_t b = 0; b < 4 ; ++b)
            std::cout << (int)a << '\t'<< (int)b << '\t'<< ((a^b)==1) << '\t' << std::endl;
0	0	0
0	1	1
0	2	0
0	3	0
1	0	1
1	1	0
1	2	0
1	3	0
2	0	0
2	1	0
2	2	0
2	3	1
3	0	0
3	1	0
3	2	1
3	3	0
*/

static std::mutex path_write;

static std::vector<std::future<void>> futures;

// NEED TO VERIFY FLOW CONTROL
// EXHAUST ALL OF THE POSSIBILITIES (BREAKING AT THE RIGHT POINT)
void find_path(const std::string& height_map_ref, std::deque<size_t> pos_history, size_t* min_length, size_t curr_pos, char ending_char, uint8_t prev_dir = 4) //copying queue over
{
    char curr_char = height_map_ref[curr_pos];
    for (uint8_t i = 0; i < 4; ++i)
    {
        if ((i^prev_dir)==1)
            continue;
        if (!tests[i](curr_pos))
            continue;
        size_t new_pos = moves[i](curr_pos);
        bool not_present = std::find(pos_history.cbegin(), pos_history.cend(), new_pos) == pos_history.cend();
        if (not_present && can_reach(curr_char, height_map_ref[new_pos]))
        {
            //prev_dir = i;
            //curr_pos = new_pos;
            //curr_char = new_char;
            //pos_history_new_path.emplace_front(new_pos);
            //size_t len_cnt = pos_history_new_path.size();
            std::deque pos_history_new_path(pos_history.begin(), pos_history.end());
            char new_char = height_map_ref[new_pos];
            pos_history_new_path.emplace_front(new_pos);
            size_t len_cnt = pos_history_new_path.size();
            if (len_cnt > *min_length)
                break;
            if (new_char == ending_char)
            {
                //std::cout << new_pos << std::endl;
                std::lock_guard<std::mutex> lock(path_write);
                std::for_each(pos_history.crbegin(), pos_history.crend(), [](const size_t& c) {std::cout << c << ' ';});
                std::cout << std::endl;
                if (len_cnt < *min_length)
                    *min_length = len_cnt;
                futures.push_back(std::async(std::launch::async, find_path, height_map_ref, pos_history_new_path, min_length, new_pos, ending_char, i));
            }

            std::lock_guard<std::mutex> lock(path_write);
            futures.push_back(std::async(std::launch::async, find_path, height_map_ref, pos_history_new_path, min_length, new_pos, ending_char,i)); //not updating the pointer/min length
        }
    }
}

size_t find_path_length(const std::string& height_map, char starting_char = 'S', char ending_char = 'E')
{
    std::deque<size_t> pos_history;
    size_t starting_pos = height_map.find(starting_char);
    if (starting_pos == std::string::npos)
        return 0;
    size_t path_len = row_length * tot_rows;
    pos_history.emplace_front(starting_pos);
    futures.push_back(std::async(std::launch::async, find_path, height_map, pos_history, &path_len, starting_pos, 'E',4));
    size_t checked = 0; //there's at least one element because I pushed it to the vector in the line above
    bool finished = false;
    
    while (!finished) {
        futures.at(checked++).get();
        if (checked == futures.size())
            finished = true;
    }

    return path_len-1; //move from 0 to 1 counts as 1st step
}





int main(int argc, char** argv)
{
    /*std::cout << COLOR_TEXT<3, 4>("HELLOOOO") << std::endl;
    sum_str(std::cout, "Uno", "due", COLOR_TEXT<4,3>("tre"), COLOR_TEXT<3, 4>("QUATTRO"));*/
    std::ifstream input(FILE_PATH<12>("input"), std::ifstream::in);
    if (!input)
        return 1;
    //get total number of spots characters
    input.seekg(0, input.end);
    auto tot_chars = input.tellg();
    input.seekg(0, input.beg);
    std::string height_map(tot_chars, '\0'); //no string termination because I'm crazy
    input.read(&height_map[0], tot_chars);
    row_length = height_map.find('\n'); //first occurrence of newline
    auto without_newline = std::remove(height_map.begin(), height_map.end(), '\n');
    height_map.erase(without_newline, height_map.end());
    tot_rows = height_map.length() / row_length; //std::distance(height_map.begin(), heightmap.end());
    std::cout << "The map measures " << COLOR_TEXT<3, 4>(row_length) << " x " << COLOR_TEXT<3, 4>(tot_rows) << std::endl;
    //sum_str(std::cout, std::boolalpha, is_adjacent(2, 3), is_adjacent(2, 4), is_adjacent(0, 8));

    /*for (char a = 97; a < 101; ++a)
        for (char b = 97; b < 101; ++b)
            std::cout << a << '\t' << b << '\t' << can_reach(a, b) << std::endl;*/
    
    auto result1 = find_path_length(height_map);

    std::cout << result1 << std::endl;

    //PLAN:
    //Find the Start position (pair of char and position) and store it somewhere
    //func(char starting_char, ending_char, policy); 
    //Run the different "paths" asynchronously
    //When the path cannot move or its length is longer than the previously found shortest path, exit the loop
    //When the thing arrives at the destination, mutex lock and write the new length
    //Need a way to ensure the paths are unique and that they don't get stuck moving back and forth (L-R, U-D)
}