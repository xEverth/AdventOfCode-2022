#include "helper.h"
#include <vector>
#include <optional>
#include <variant>
#include <charconv>
#include <compare>
#include <set>

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
    bool operator==(const List& other) const = default;
    std::strong_ordering operator<=>(const List& other) const {
        size_t size1 = content.size(), size2 = other.content.size();
        size_t size_max = size2 > size1 ? size2 : size1;
        for (size_t i = 0; i < size_max; ++i) {
            if (i >= size2) //RIGHT is over before LEFT
                return std::strong_ordering::greater;
            if (i >= size1) //LEFT is over before RIGHT
                return std::strong_ordering::less; // &true?
            if (std::holds_alternative<optInt>(content.at(i)) && std::holds_alternative<optInt>(other.content.at(i))) { //If both items are Integers
                if (std::get<optInt>(content.at(i)).value_or(0) != std::get<optInt>(other.content.at(i)).value_or(0))
                    return std::get<optInt>(content.at(i)).value_or(0) <=> std::get<optInt>(other.content.at(i)).value_or(0);
            } else if (std::holds_alternative<optList>(content.at(i)) && std::holds_alternative<optList>(other.content.at(i))) //Else if both items are Lists
            {
                if (std::get<optList>(content.at(i)).has_value() && std::get<optList>(other.content.at(i)).has_value())
                {
                    if ((std::get<optList>(content.at(i)).value() != std::get<optList>(other.content.at(i)).value()))
                        return std::get<optList>(content.at(i)).value() <=> std::get<optList>(other.content.at(i)).value();
                }
                else if (!std::get<optList>(content.at(i)).has_value() && !std::get<optList>(other.content.at(i)).has_value()) //If both lists are empty
                    continue;
                else if (std::get<optList>(content.at(i)).has_value() && !std::get<optList>(other.content.at(i)).has_value()) //If RIGHT is an empty list
                    return std::strong_ordering::greater;
                else if (!std::get<optList>(content.at(i)).has_value() && std::get<optList>(other.content.at(i)).has_value()) //If LEFT is an empty list
                    return std::strong_ordering::less;
            }
            else if (std::holds_alternative<optList>(content.at(i))) //If LEFT item is a List, RIGHT item is an Integer
            {
                if (std::get<optList>(content.at(i)).has_value())
                {
                    auto& Left_list = std::get<optList>(content.at(i)).value();
                    if (Left_list != List(std::get<optInt>(other.content.at(i)).value_or(0)))
                        return Left_list <=> List(std::get<optInt>(other.content.at(i)).value_or(0));
                }
                else //LEFT is an empty list
                    return std::strong_ordering::less;
            }
            else //RIGHT item is a List, LEFT item is an Integer
            {
                if (std::get<optList>(other.content.at(i)).has_value())
                {
                    auto& Right_list = std::get<optList>(other.content.at(i)).value();
                    if (List(std::get<optInt>(content.at(i)).value_or(0)) != Right_list)
                        return List(std::get<optInt>(content.at(i)).value_or(0)) <=> Right_list;
                }
                else //RIGHT is an empty list
                    return std::strong_ordering::greater;
            }
        }
        return std::strong_ordering::equal;
    }
    explicit List(uint32_t val) : content{ val } {};
    List(std::string inputstr) noexcept : content{}
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
                    i += 2;
                    continue;
                }
                std::string inner_list = inputstr.substr(i+1, next_close-i-1); //what if the substr doesn't end with a close?
                content.emplace_back(List(inner_list));
                next_comma = inputstr.find(',', next_close);
                i = next_close + 1;
                if (i == next_comma)
                    continue;
            }
            uint32_t curr = 0;


            if (next_comma != std::string::npos)
            {
                auto convres = std::from_chars(inputstr.data() + i, inputstr.data() + next_comma+1, curr);
                if (convres.ec != std::errc::invalid_argument)
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
            result += "[]"; // {empty}
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
    std::ifstream input(FILE_PATH<13>("input"), std::ifstream::in);
    uint32_t result1 = 0, index = 1;
    auto less = [](const List& first, const List& second) {return (first <=> second)<0;};
    std::multiset<List, decltype(less) > all_signals;
    std::string a, b, nl;
    while (input)
    {
        std::getline(input, a);
        std::getline(input, b);
        std::getline(input, nl);
        List list1(a);
        List list2(b);
        //std::cout << std::boolalpha
        //    << "variant has any values?"
        //    << std::get<std::optional<List>>(list1.content[0]).has_value() << '\n';
        //std::cout   << /*"list1: " <<*/ printList(list1.content[0]) << '\n'
        //            << /*"list2: " <<*/ printList(list2.content[0]) << '\n' << std::endl;
        if (list1 < list2)
        {
            //std::cout << "True: List 1 < List 2 \t Index: " << index << '\n';
            result1 += index;
        }
        //else if (list1 > list2)
        //    std::cout << "False: List 1 > List 2\n";
        //else
        //    std::cout << "False: List 1 == List 2\n";
        ++index;

        all_signals.insert(a);
        all_signals.insert(b);
    }
    all_signals.emplace("[[2]]");
    all_signals.emplace("[[6]]");
    auto pos2 = all_signals.upper_bound(std::string("[[2]]"));
    auto pos6 = all_signals.upper_bound(std::string("[[6]]"));
    std::cout << "Result 1: " << result1 << std::endl;
    std::cout << "Result 2: " << (std::distance(all_signals.begin(), pos2)) * (std::distance(all_signals.begin(), pos6)) << std::endl;

    //for (auto it = all_signals.begin(); it != all_signals.end(); ++it)
    //{
    //    std::cout << std::distance(all_signals.begin(), it) +1 << ": " << printList(it->content[0]) << std::endl;
    //}
}