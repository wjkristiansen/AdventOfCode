#include "pch.h"
#include "SolutionFactory.h"

class CSolution12: public CSolutionBase
{
    void Execute(int)
    {
        std::ifstream fstream("2023/Day12.txt");

        std::regex rowPattern("([?#.]+)\\s(.+)");
        std::regex numbersPattern("(\\d+)(?:,|$)", std::regex_constants::ECMAScript);

        for (; !fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);
            if (line.empty())
                continue; // Skip blank lines
            std::smatch rowData;
            std::regex_search(line, rowData, rowPattern);
            std::string springsString = rowData[1].str();
            std::string numbersString = rowData[2].str();
            std::vector<int64_t> numbers;
            auto numbersBegin = std::sregex_iterator(numbersString.begin(), numbersString.end(), numbersPattern);
            auto numbersEnd = std::sregex_iterator();
            for (auto numbersIt = numbersBegin; numbersIt != numbersEnd; ++numbersIt)
            {
                numbers.push_back(std::stoi(numbersIt->str()));
            }

            std::cout << springsString << ": ";
            for (auto n : numbers)
            {
                std::cout << n << ", ";
            }
            std::cout << std::endl;

            // Build a mask of known bad springs
            int64_t knownBadMask = 0;
            for (auto it = springsString.begin(); it != springsString.end(); ++it)
            {
                knownBadMask = knownBadMask << 1;
                knownBadMask |= *it == '#' ? 1 : 0;
            }

            // Build group masks
            std::vector<int64_t> groupMasks(numbers.size());
            for (int i = 0; i < numbers.size(); ++i)
            {
                groupMasks[i] = (1 << numbers[i]) - 1;

                std::cout << std::bitset<16>(groupMasks[i]) << std::endl;
            }
        }
    }
};

DECLARE_SOLUTION(CSolution12, 12, "Hot Springs");