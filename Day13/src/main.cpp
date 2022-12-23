#include "helper.h"
#include <vector>
#include <optional>
#include <variant>
#include <charconv>


struct List
{
    std::vector < std::variant < std::optional<uint32_t>, std::optional < List > > > content;
    List(std::string inputstr) : content{}
    {
        char level = 0; // when it's back at zero, the object is constructed
        for (size_t i = 0; i < inputstr.size(); ++i) //if string is empty, loop is not entered, so only a "std::optional<List> {}" is constructed in the vector
        {

            size_t next_comma = inputstr.find(',', i);
            size_t next_close = inputstr.find(']', i);

            if (inputstr.at(i) == '[')
            {
                ++level;
                size_t last_close = inputstr.rfind(']');
                if (i + 1 == last_close)
                {
                    content.emplace_back(std::optional<List> {});
                    break;
                }
                std::string inner_list = inputstr.substr(i+1, last_close-1);
                content.emplace_back(List(inner_list));
                next_comma = inputstr.find(',', last_close);
                i = last_close + 1;
                if (i == next_comma)
                    continue;
            }
            uint32_t curr = 0;
            /*if (inputstr.at(i) == ']')
                break;*/


            if (next_comma != std::string::npos)
            {
                auto convres = std::from_chars(inputstr.data() + i, inputstr.data() + next_comma, curr);
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

std::string printList(std::variant < std::optional<uint32_t>, std::optional < List > > it)
{ //need to handle the nullopt cases by printing nothing
    std::string result;
    if (std::holds_alternative<std::optional<uint32_t>>(it))
    {
        result += std::to_string(std::get<std::optional<uint32_t>>(it).value()) + ", ";
    }
    else if (std::holds_alternative<std::optional < List > >(it))
    {
        result += "[";
        for (const auto& el : std::get<std::optional < List > >(it).value().content)
        {
            result += printList(el);
        }
        result += "]";
    }
    else
    {
        std::cout << "I FD UP" << std::endl;
    }
    return result;
}

//decltype(auto) getNum(std::variant < std::optional<uint32_t>, std::optional < List > > it)
//{
//    return std::get<std::optional<uint32_t>>(it).value();
//}
//
//decltype(auto) getList(std::variant < std::optional<uint32_t>, std::optional < List > > it)
//{
//    return std::get<std::optional<List>>(it).value();
//}

int main(int argc, char** argv) {
    std::ifstream input(FILE_PATH<13>("test"), std::ifstream::in);
    std::string a, b, nl;
    while (input)
    {
        input >> a >> b >> nl;
        List list1(a);
        std::cout << std::boolalpha
            << "variant has any values?"
            << std::get<std::optional<List>>(list1.content[0]).has_value() << "\n";
        std::cout << printList(list1) << std::endl;
    }
}