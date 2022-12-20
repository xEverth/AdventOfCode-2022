#include <fstream>
#include <iostream>
#include <string>
#include <numeric>
#include <iterator>

template <int N>
constexpr auto FILE_PATH()
{
    return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + "input.txt";
}

int main(int argc, char** argv)
{
    std::ifstream input(FILE_PATH<8>(), std::istream::in);
    if (!input)
        return 1;
    //get total number of trees
    input.seekg(0, input.end);
    int numTrees = input.tellg();
    input.seekg(0, input.beg);
    std::string forest(numTrees, '\0'); //no string termination because I'm crazy
    input.read(&forest[0], numTrees);
    //std::cout << forest << std::endl;
    size_t row_length = forest.find('\n'); //first occurrence of newline
    auto withoutNewline = std::remove(forest.begin(), forest.end(), '\n');
    forest.erase(withoutNewline, forest.end());
    int visible = 0, scenic_score = 1;
    auto tot_rows = forest.length() / row_length; //std::distance(forest.begin(), forest.end());
    for (auto it = forest.begin(); it != forest.end(); std::advance(it,1))
    {
        auto distance_from_start = std::distance(forest.begin(), it);
        auto distance_from_end = std::distance(it, forest.end());
        auto distance_from_row_start = distance_from_start % row_length;
        auto curr_row = distance_from_start / row_length;

        //BORDER TESTING
        bool first_col = (distance_from_row_start == 0),
            last_col = (distance_from_row_start == ((row_length - 1) % row_length)),
            first_row = (distance_from_start < row_length),
            last_row = (distance_from_end < row_length);
        bool is_side = first_col || last_col || first_row || last_row;
        
        //VIEW TESTING
        auto visible_from_top = [&distance_from_row_start, &curr_row, &tot_rows, &row_length](std::string::iterator iter) -> bool { //get iter by copy
            bool visible = true;
            char tree_height = *iter;
            for (size_t i = 1; i < curr_row+1; ++i) {
                visible &= tree_height > *(std::prev(iter, i * row_length));
            }
            return visible;
        };

        auto visible_from_bot = [&distance_from_row_start, &curr_row, &tot_rows, &row_length](std::string::iterator iter) -> bool { //get iter by copy
            bool visible = true;
            char tree_height = *iter;
            for (size_t i = 1; i < (tot_rows-curr_row); ++i) {
                visible &= tree_height > *(std::next(iter, i * row_length));
            }
            return visible;
        };

        auto visible_from_left = [&distance_from_row_start, &curr_row, &tot_rows, &row_length](std::string::iterator iter) -> bool { //get iter by copy
            bool visible = true;
            char tree_height = *iter;
            for (size_t i = 1; i < distance_from_row_start+1; ++i) {
                visible &= tree_height > *(std::prev(iter, i));
            }
            return visible;
        };

        auto visible_from_right = [&distance_from_row_start, &curr_row, &tot_rows, &row_length](std::string::iterator iter) -> bool { //get iter by copy
            bool visible = true;
            char tree_height = *iter;
            for (size_t i = 1; i < row_length-distance_from_row_start; ++i) {
                visible &= tree_height > *(std::next(iter, i));
            }
            return visible;
        };


        if (is_side || visible_from_top(it) || visible_from_left(it) || visible_from_bot(it) || visible_from_right(it) )
        {
            visible += 1;
            //std::cout << "\033[1;31m" << *it << "\033[0m";
            if (last_col);
                //std::cout << " ROW N: " << curr_row << std::endl;
        }
        else {
            //std::cout << *it;
        }

        //Calculate the scenic score
        char tree_height_minus_one = *it-1; //slightly faster than doing >= on every cycle
        std::string::iterator iter = it;
        int distances[] = { 0,0,0,0 }; //TOP, BOT, LEFT, RIGHT
        int curr_scenic_score = 1;
        //DISTANCE FROM TOP
        for (size_t i = 1; i < curr_row + 1; ++i) {
            ++distances[0];
            if (*(std::prev(iter, i * row_length)) > tree_height_minus_one)
                break;
        }
        iter = it;
        //DISTANCE FROM BOT
        for (size_t i = 1; i < (tot_rows - curr_row); ++i) {
            ++distances[1];
            if ( *(std::next(iter, i * row_length)) > tree_height_minus_one)
                break;
        }
        iter = it;
        //DISTANCE FROM LEFT
        for (size_t i = 1; i < distance_from_row_start + 1; ++i) {
            ++distances[2];
            if ( *(std::prev(iter, i)) > tree_height_minus_one)
                break;
        }
        iter = it;
        //DISTANCE FROM RIGHT
        for (size_t i = 1; i < row_length - distance_from_row_start; ++i) {
            ++distances[3];
            if ( *(std::next(iter, i)) > tree_height_minus_one)
                break;
        }
        std::cout << *it << " : ";
        for (const int& num : distances)
        {

            std::cout << num << ' ';
            curr_scenic_score *= num;
        }
        std::cout << "\t";
        if (curr_scenic_score > scenic_score)
            scenic_score = curr_scenic_score;
    }
    //std::cout << std::endl << forest << std::endl;
    std::cout << "trees on the sides: " << visible << std::endl
        << "maximum scenic score: " << scenic_score << std::endl;
}