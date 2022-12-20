#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <charconv>
#include <algorithm>
#include <numeric>
#include <functional>

template <int N>
constexpr auto FILE_PATH(std::string name = "input") // "test" or "input"
{
    return "C:\\Code\\AdventOfCode\\Day" + std::to_string(N) + "\\src\\" + std::to_string(N) + name + ".txt";
}

//  "\033[1;31m" << *it << "\033[0m"

//A-B TESTING FOR SPEED

#define TIMER 1
#if TIMER == 1
#include <chrono>
#include <thread>

struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;
    std::string sect_name = "Main";

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    Timer(std::string name) : sect_name(name)
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << sect_name << " took: " << ms << "ms " << std::endl;
    }
};
#endif

#define A 2
#define B 2

#if B == 1
typedef unsigned long long ull;
#endif
#if B == 2
typedef uint64_t ull;
#endif
#if B == 3
typedef int64_t ull;
#endif

class Monkey
{
    int id = 0;
    std::vector<ull> items;
    ull divisor = 1;
    int id_if_true = 0;
    int id_if_false = 0;
    ull items_thrown = 0;
    std::string op_line;
    std::function<ull(ull)> op;
#if A == 1
    ull execute_op(ull curr, std::string buf1, char operation, std::string buf2) {
        //std::cout << "OPERATION:" << '\n' << buf1 << '\t' << operation << '\t' << buf2 << std::endl;
        switch (operation)
        {
        case '+':
            return (buf1 == "old") ?
                curr + ((buf2 == "old") ? curr : std::stoi(buf2))
                :
                std::stoi(buf1) + ((buf2 == "old") ? curr : std::stoi(buf2));
        case '-':
            return (buf1 == "old") ?
                curr - ((buf2 == "old") ? curr : std::stoi(buf2))
                :
                std::stoi(buf1) - ((buf2 == "old") ? curr : std::stoi(buf2));
        case '*':
            return (buf1=="old") ?
                curr * ((buf2 == "old") ? curr : std::stoi(buf2))
                :
                std::stoi(buf1) * ((buf2=="old") ? curr : std::stoi(buf2));
        case '/':
            return (buf1 == "old") ?
                curr / ((buf2 == "old") ? curr : std::stoi(buf2))
                :
                std::stoi(buf1) / ((buf2 == "old") ? curr : std::stoi(buf2));
        default:
            return -1;
        }
    }
    void ParseOperation() {
        std::stringstream ss(op_line);
        std::string buf1, buf2;
        char operation;
        ss >> buf1 >> operation >> buf2;
        op = std::bind(&Monkey::execute_op, this, std::placeholders::_1, buf1, operation, buf2);
    }
#endif
#if A == 2 //replace return call with bind to execute_op with std::placeholder_1
    ull execute_binary_op(ull operand1, char operation, ull operand2) {
        //std::cout << "OPERATION:" << '\n' << buf1 << '\t' << operation << '\t' << buf2 << std::endl;
        switch (operation)
        {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            return operand1 / operand2;
        default:
            return -1;
        }
    }
    void ParseOperation() {
        std::stringstream ss(op_line);
        std::string buf1, buf2;
        char operation;
        ss >> buf1 >> operation >> buf2;
        if (buf1 == "old")
        {
            if (buf2 == "old")
            {
                op = std::bind(&Monkey::execute_binary_op, this, std::placeholders::_1, operation, std::placeholders::_1);
            }
            else //buf2 contains an int
            {
                op = std::bind(&Monkey::execute_binary_op, this, std::placeholders::_1, operation, std::stoi(buf2));
            }
        } 
        else //buf1 contains an int
        {
            if (buf2 == "old")
            {
                op = std::bind(&Monkey::execute_binary_op, this, std::stoi(buf1), operation, std::placeholders::_1);
            }
            else //buf2 contains an int
            {
                op = std::bind(&Monkey::execute_binary_op, this, std::stoi(buf1), operation, std::stoi(buf2));
            }
        }
    }
#endif
    friend std::ifstream& operator>>(std::ifstream& in, Monkey& m1);
public:
    Monkey() = default;
    void PlayTurn(std::vector<Monkey>& players, ull cycle_length = 0)
    {
#if TIMER == 1
        //Timer turn("Turn of Monkey" + std::to_string(this->id));
#endif
        std::for_each(items.begin(), items.end(), [this, &players, cycle_length](ull worry)
            {
                //std::cout << "Monkey " << "\033[1;33m" << id << "\033[0m" << " inspects item with worry level of " << worry << '\n' << "Worry level becomes: " << op(worry) << " and then " << op(worry) / 3 << '\n' << " The worry is " << (op(worry) / 3 % divisor == 0 ? "" : "NOT ") << "divisible by " << divisor << ", so it's going to monkey " << (op(worry) / 3 % divisor == 0 ? id_if_true : id_if_false) << '\n' << std::endl
                ull new_worry = 0;
                if (cycle_length == 0)
                    new_worry = op(worry) / 3;
                else
                {
#if TIMER == 1 
                    //Timer part2op("Part2 operation on worry, Monkey " + std::to_string(id)); 
#endif
                    new_worry = op(worry) % cycle_length;
                    //subtracting the product of all the dividends does not alter the reminder since the operations sum and product commute with the modulo
                    //The below part should be mathematically equal to the above but it's thousands of times slower for monkey 3 (squaring big numbers)
                    //while (new_worry > cycle_length)
                        //new_worry -= cycle_length;
                }
#if TIMER == 1
                //Timer vectorpush("Vector push");
#endif
                (new_worry % divisor) == 0 ? players[id_if_true].items.push_back(new_worry) : players[id_if_false].items.push_back(new_worry);
                items_thrown += 1;
            });
        items.clear();
    }
    ull get_thrown() const 
    {
        return items_thrown;
    }
    ull get_divisor() const
    {
        return divisor;
    }
};

std::ifstream& operator>>(std::ifstream& in, Monkey& m1) {
    std::string buf;
    int num;
    std::getline(in, buf);
    //PARSING INPUT
    //MONKEY NUMBER
    size_t wspos = buf.find(' '), colonpos = buf.find(':');
    //CONVERTS WHATEVER'S AFTER THE WHITESPACE AND BEFORE THE COLON TO INT
    auto conv_result = std::from_chars(&buf[wspos+1], &buf[colonpos], m1.id);
    //std::cout << m1.id;
    //MONKEY ITEMS
    std::getline(in, buf);
    auto withoutNewline = std::remove(buf.begin(), buf.end(), ',');
    buf.erase(withoutNewline, buf.end());
    colonpos = buf.find(':');
    std::stringstream ss(buf.substr(colonpos + 2, buf.size()));
    m1.items.reserve(16); //increasing capacity to 16 ( 8*16 bytes)
    while (ss >> num)
        m1.items.push_back(static_cast<ull>(num)); // explicit conversion, possibly pointless
    //std::string numlist = buf.substr(colonpos+2, buf.size()); //buf.copy(numlist.data(), buf.size() - colonpos, colonpos+2);
    //auto withoutNewline = std::remove(numlist.begin(), numlist.end(), ',');
    //numlist.erase(withoutNewline, numlist.end());
    //PARSE OP
    std::getline(in, buf);
    size_t equalpos = buf.find('=');
    m1.op_line = buf.substr(equalpos + 2, buf.size());
    m1.ParseOperation();
    //std::cout << "result of operation on 1 is: " << m1.op(1) << std::endl;
    //PARSE TEST
    std::getline(in, buf);
    wspos = buf.rfind(" ");
    conv_result = std::from_chars(&buf[wspos + 1], &buf[buf.length()], m1.divisor);
    //PARSE MONKEY_IF_TRUE
    std::getline(in, buf);
    wspos = buf.rfind(" ");
    conv_result = std::from_chars(&buf[wspos + 1], &buf[buf.length()], m1.id_if_true);
    //PARSE MONKEY_IF_FALSE
    std::getline(in, buf);
    wspos = buf.rfind(" ");
    conv_result = std::from_chars(&buf[wspos + 1], &buf[buf.length()], m1.id_if_false);
    //IF there's more, eat newline
    return in;
}


int main(int argc, char** argv)
{
#if TIMER == 1
    Timer timer;
#endif
    std::ifstream input(FILE_PATH<11>("input"), std::ifstream::in);
    std::vector<Monkey> scimmie_vector;
    std::string nlbuf(1, '\0');
    while (input)
    {
        Monkey scimmia_input;
        input >> scimmia_input;
        //scimmie_vector.push_back(scimmia_input);
        scimmie_vector.emplace_back(std::move(scimmia_input));
        input >> nlbuf;
    }
    std::vector<ull> results1, results2;
#if TIMER == 1
    Timer part1("Part1");
#endif
    //PART1
    constexpr uint32_t part1_rounds = 20;
    std::vector<Monkey> scimmie_vector2(scimmie_vector.begin(), scimmie_vector.end()); //copy before the edits
    for (size_t round = 0; round < part1_rounds; ++round)
    {
        std::for_each(scimmie_vector.begin(), scimmie_vector.end(), [&scimmie_vector](Monkey& monkey) 
            {
                monkey.PlayTurn(scimmie_vector,0); 
            });
    }
#if TIMER == 1
    part1.~Timer();

    Timer part2("Part2");
#endif
    //BY MULTIPLYING TOGETHER ALL DIVISORS, I'M SURE I WON'T AFFECT THE DIVISIBILITY TEST FOR ANY MONKEY, REGARDLESS OF THE OPERATIONS PERFORMED
    ull cycle_length = std::accumulate(scimmie_vector2.begin(), scimmie_vector2.end(), ull(1), [](ull prod, const Monkey& m1) 
        {
            //std::cout << "PROD: " << prod << '\t' << m1.get_divisor() << std::endl << prod * m1.get_divisor() << '\n' << std::lcm(prod, m1.get_divisor()) << std::endl;
            //return std::lcm(prod, m1.get_divisor()) gives the same result since the numbers are co-prime
            return prod * m1.get_divisor();
        });
    //std::cout << cycle_length << std::endl;

    //PART2 10.000 TAKES 500 SECS IN DEBUG MODE AND AROUND 80 IN RELEASE
    constexpr uint32_t part2_rounds = 10'000;
    for (uint32_t round = 0; round < part2_rounds; ++round)
    {
        std::for_each(scimmie_vector2.begin(), scimmie_vector2.end(), [&scimmie_vector2, cycle_length](Monkey& monkey)
            {
                monkey.PlayTurn(scimmie_vector2, cycle_length);
            });
    }
    for (uint32_t id = 0; id < scimmie_vector.size(); ++id)
    {
        //std::cout << "Part1\nMonkey " << id << " Thrown " << scimmie_vector.at(id).get_thrown() << " Items" << std::endl;
        results1.push_back(scimmie_vector.at(id).get_thrown());

        //std::cout << "Part2\nMonkey " << id << " Thrown " << scimmie_vector2.at(id).get_thrown() << " Items" << std::endl;
        results2.push_back(scimmie_vector2.at(id).get_thrown());
    }
    std::sort(results1.begin(), results1.end(), std::greater<ull>());
    std::cout << "Part1\nProduct of two biggest numbers: " << results1[0] * results1[1] << std::endl;

    std::sort(results2.begin(), results2.end(), std::greater<ull>());
    //uint64_t monkey_business_2 = static_cast<uint64_t>(results2[0]) * results2[1];
    /*std::cout << "Number of throws in scenario 2, sorted " << std::endl;
    for (auto it = results2.rbegin(); it != results2.rend();++it)
    {
        std::cout << *it << '\t';
    } std::cout << std::endl;*/
    std::cout << "Part2\nProduct of two biggest numbers: " << results2[0] * results2[1] << std::endl;
#if TIMER == 1
    part2.~Timer();
#endif
}