#include "pch.h"
#include "SolutionFactory.h"
#include <cmath>

class CSolution7: public CSolutionBase
{
public:
    CSolution7() : CSolutionBase()
    {
    }

    enum class Operation
    {
        Add,
        Mul,
        Concat
    };

    static size_t Concatenate(size_t v1, size_t v2)
    {
        size_t temp = v2;
        size_t digits = 0;

        while (temp)
        {
            temp /= 10;
            digits++;
        }

        uint64_t multiplier = static_cast<size_t>(std::pow(10, digits));
        return v1 * multiplier + v2;
    }

    static bool FindEquation(
        size_t test,
        size_t accum,
        Operation op,
        std::vector<size_t>::const_iterator first,
        std::vector<size_t>::const_iterator last)
    {
        size_t Value = *first;
        switch (op)
        {
        case Operation::Add:
            accum += Value;
            break;
        case Operation::Mul:
            accum *= Value;
            break;
        case Operation::Concat:
            accum = Concatenate(accum, Value);
            break;
        }
        auto next = first + 1;
        if (next == last)
        {
            // Compare to the test value
            if (accum == test)
                return true;
        }
        else
        {
            if (FindEquation(test, accum, Operation::Add, next, last))
                return true;
            if (FindEquation(test, accum, Operation::Mul, next, last))
                return true;
            if (FindEquation(test, accum, Operation::Concat, next, last))
                return true;
        }
        return false;
    }

    virtual void Execute(int part)
    {
        int minPart = part > 0 ? part : 1;
        int maxPart = part > 0 ? part : 2;
        for (int runPart = minPart; runPart <= maxPart; ++runPart)
        {
            if (runPart == 1)
            {
                std::ifstream fstream("2024/Day7.txt");
                size_t sum = 0;
                for (int i = 0;!fstream.eof();++i)
                {
                    std::string line;
                    std::getline(fstream, line);
                    std::istringstream ss(line);

                    size_t output;
                    std::vector<size_t> inputs;

                    ss >> output;
                    ss.ignore(1, ':');
                    ss.ignore(1, ' ');
                    std::string operandString;
                    while (std::getline(ss, operandString, ' '))
                    {
                        inputs.push_back(std::stoi(operandString));
                    }
                }
                std::cout << "Part 1: Sum: " << sum << std::endl;
            }
            else
            {
                std::ifstream fstream("2024/Day7.txt");
                size_t sum = 0;
                for (int i = 0;!fstream.eof();++i)
                {
                    std::string line;
                    std::getline(fstream, line);
                    std::istringstream ss(line);

                    size_t output;
                    std::vector<size_t> inputs;

                    ss >> output;
                    ss.ignore(1, ':');
                    ss.ignore(1, ' ');
                    std::string operandString;
                    while (std::getline(ss, operandString, ' '))
                    {
                        inputs.push_back(std::stoi(operandString));
                    }

                    if (FindEquation(output, 0, Operation::Add, inputs.begin(), inputs.end()))
                        sum += output;
                }
                std::cout << "Part 2: Sum: " << sum << std::endl;
            }
        }
    }
};

DECLARE_SOLUTION(CSolution7, 7, "Bridge Repair");
