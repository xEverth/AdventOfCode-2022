#include "helper.h"
#include <vector>
#include <optional>
#include <variant>
#include <charconv>
#include <iterator>
#include <numeric>

//When comparing two values, the first value is called leftand the second value is called right.Then:
//If both values are integers, the lower integer should come first.If the left integer is lower than the right integer, the inputs are in the right order.If the left integer is higher than the right integer, the inputs are not in the right order.Otherwise, the inputs are the same integer; continue checking the next part of the input.
//If both values are lists, compare the first value of each list, then the second value, and so on.If the left list runs out of items first, the inputs are in the right order.If the right list runs out of items first, the inputs are not in the right order.If the lists are the same lengthand no comparison makes a decision about the order, continue checking the next part of the input.
//If exactly one value is an integer, convert the integer to a list which contains that integer as its only value, then retry the comparison.For example, if comparing[0, 0, 0] and 2, convert the right value to[2](a list containing 2); the result is then found by instead comparing[0, 0, 0] and [2].

struct List
{
    typedef std::optional<uint32_t> optInt;
    typedef std::optional < List > optList;
    typedef std::vector < std::variant < optInt, optList > > ListContent;
    ListContent content;
    bool operator<(const List& other) {
        //implement operation comparison
    }
    List(std::string inputstr) : content{}
    {
        char level = 0; //used to count the indentation level
        for (size_t i = 0; i < inputstr.size(); ++i) //if string is empty, loop is not entered, so only a "std::optional<List> {}" is constructed in the vector
        {
            size_t next_comma = inputstr.find(',', i+1);
            if (inputstr.at(i) == '[')
            {
                size_t inner_pos = i, next_open, next_close;
                do
                {
                    next_open = inputstr.find('[', inner_pos);
                    next_close = inputstr.find(']', inner_pos);

                    if (next_open < next_close && next_open != std::string::npos)
                    {
                        ++level;
                        inner_pos = next_open + 1;
                    }
                    else if (next_close != std::string::npos)
                    {
                        --level;
                        inner_pos = next_close + 1;
                    }
                } while (level != 0);

                if (i + 1 == next_close)
                {
                    content.emplace_back(std::optional<List> {});
                    break;
                }
                std::string inner_list = inputstr.substr(i+1, next_close-i-1); //what if the substr doesn't end with a close?
                content.emplace_back(List(inner_list));
                next_comma = inputstr.find(',', next_close);
                i = next_close + 1;
                if (i == next_comma)
                    continue;
            }
            uint32_t curr = 0;
            /*if (inputstr.at(i) == ']')
                break;*/


            if (next_comma != std::string::npos)
            {
                auto convres = std::from_chars(inputstr.data() + i, inputstr.data() + next_comma+1, curr);
                content.emplace_back(curr);
                i = next_comma;
            }
            else if (i != inputstr.size() )
            {
                auto convres = std::from_chars(inputstr.data() + i, inputstr.data() + inputstr.size(), curr);
                content.emplace_back(curr);
                ++i;
            }
        }
    }
};

//void printList(const List& list)
//{
//    for (auto& el : list.content)
//    {
//        printList(el);
//    }
//}

std::string printList( const std::variant < List::optInt, List::optList>& it, std::string sep = ",")
{ //need to handle the nullopt cases by printing nothing
    std::string result;
    if (std::holds_alternative<List::optInt>(it))
    {
        result += std::to_string(std::get<List::optInt>(it).value());
    }
    else if (std::holds_alternative<std::optional < List > >(it))
    {
        if ((std::get<List::optList>(it).has_value()))
        {
        result += "[";
            for (const auto& el : std::get<List::optList>(it).value().content)
            {
                result += printList(el) + sep;
            }
            result.pop_back();
            result += "]";
        }
        else {
            result += "[{empty}]";
        }
    }
    else
    {
        std::cout << "I FD UP" << std::endl;
    }
    return result;
}

List::optInt getInt(std::variant < List::optInt, List::optList > self)
{
    return std::get<List::optInt>(self).value();
}

int main(int argc, char** argv) {
    std::ifstream input(FILE_PATH<13>("test"), std::ifstream::in);
    std::string a, b, nl;
    while (input)
    {
        std::getline(input, a);
        std::getline(input, b);
        std::getline(input, nl);
        List list1(a);
        List list2(b);
        /*std::cout << std::boolalpha
            << "variant has any values?"
            << std::get<std::optional<List>>(list1.content[0]).has_value() << '\n';*/
        std::cout << "list1: " << printList(list1.content[0]) << '\n'
                  << "list2: " << printList(list2.content[0]) << std::endl;
    }
}