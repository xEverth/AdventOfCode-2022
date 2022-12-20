#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <numeric>
#include <future>
#include <functional>
#include <atomic>
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



uint16_t inline numeric_height(const char& curr_height)
{
    if (curr_height < 'a') //UPPERCASE in ASCII are lower in value than uppercase
    {
        if (curr_height == 'S')
            return 0;
        return (curr_height - 'E' + 26); //so A > a and E > z
    }
    else
        return (curr_height - 'a');
}

bool inline can_reach(const uint16_t& curr, const uint16_t& next)
{
    return (next - 2 - curr) < 0;
}

bool inline can_reach(const char& curr, const char& next)
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

static std::queue<std::future<void>> futures;

struct PosLenDir
{
    size_t pos;
    size_t len;
    uint8_t dir;
};

// NEED TO VERIFY FLOW CONTROL
// EXHAUST ALL OF THE POSSIBILITIES (BREAKING AT THE RIGHT POINT)
void find_path(const std::string& height_map_ref, std::queue<PosLenDir>* pos_history, std::atomic<size_t>** min_len_to_pos, const PosLenDir* curr_poslendir, size_t ending_pos) {
    char curr_char = height_map_ref[curr_poslendir->pos];
    for (uint8_t i = 0; i < 4; ++i)
    {
        if ((i^curr_poslendir->dir)==1)
            continue;
        if (!tests[i](curr_poslendir->pos))
            continue;
        size_t new_pos = moves[i](curr_poslendir->pos);
        size_t curr_min_len = (* min_len_to_pos)[new_pos].load(); //load(std::memory_order_relaxed);
        if (can_reach(curr_char, height_map_ref[new_pos]) && ((curr_poslendir->len +2) < curr_min_len) )
        {
            char new_char = height_map_ref[new_pos];
            size_t new_len = curr_poslendir->len + 1;
            size_t ending_len = (*min_len_to_pos)[ending_pos].load();
            if (new_len > ending_len) //saving it twice atm, both in the min_len_to_pos[height_map_ref.find(ending_char)] and here
                break;
            /*if (new_pos == ending_pos)
            {
                std::lock_guard<std::mutex> lock(path_write);
                if (new_len < *min_length)
                    *min_length = new_len;
                pos_history->emplace(PosLenDir(new_pos, new_len,i));
            }*/
            //std::cout << (new_pos == ending_pos) << std::endl;
            std::lock_guard<std::mutex> lock(path_write);
            (*min_len_to_pos)[new_pos].store(new_len);
            pos_history->emplace(PosLenDir(new_pos, new_len,i));
        }
    }
}

size_t find_path_length(const std::string& height_map, char starting_char = 'S', char ending_char = 'E')
{
    std::queue<PosLenDir> pos_history;
    size_t path_len = row_length * tot_rows;
    std::atomic<size_t> *dist_to_pos = new std::atomic<size_t>[path_len];
    for (size_t i = 0; i < path_len; ++i)
    {
        dist_to_pos[i].store(path_len);
    }
    size_t starting_pos = height_map.find(starting_char); //iterate over all .find
    size_t ending_pos = height_map.find(ending_char);
    size_t shortest = path_len, shortest_so_far = path_len; //inefficient: I'm repeating the whole search for all possible starting positions, while I could check if I meet another char with elevation 'a' during the path and drop out early from the loop
    while (starting_pos != std::string::npos) 
    {   
        pos_history.emplace(PosLenDir(starting_pos, 0, 4));
        size_t checked = 0; //there's at least one element because I pushed it to the vector in the line above
    
        while (!pos_history.empty())
        {
            PosLenDir curr_poslendir = pos_history.front();
            pos_history.pop();
            checked++;
            futures.emplace(std::async(std::launch::async, find_path, height_map, &pos_history, &dist_to_pos, &curr_poslendir, ending_pos));
            while (!futures.empty()) {
                futures.front().get();
                futures.pop();
            }
        }
        shortest_so_far = dist_to_pos[ending_pos].load();
        if (shortest_so_far < shortest)
            shortest = shortest_so_far;
        starting_pos = height_map.find(starting_char, starting_pos+1);
        for (size_t i = 0; i < path_len; ++i)
        {
            dist_to_pos[i].store(path_len);
        }
    }

    return shortest;
    delete[] dist_to_pos;
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

    auto result2 = find_path_length(height_map, 'a', 'E');

    std::cout << "Result 1: " << result1 << "\nResult 2: " << result2 << std::endl;

    //PLAN:
    //Find the Start position (pair of char and position) and store it somewhere
    //func(char starting_char, ending_char, policy); 
    //Run the different "paths" asynchronously
    //When the path cannot move or its length is longer than the previously found shortest path, exit the loop
    //When the thing arrives at the destination, mutex lock and write the new length
    //Need a way to ensure the paths are unique and that they don't get stuck moving back and forth (L-R, U-D)
}