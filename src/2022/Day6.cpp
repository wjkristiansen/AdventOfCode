#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <optional>

#include "..\SolutionFactory.h"

bool AreFourUnique(const std::deque<char> &lastFour)
{
    std::set<char> uniqueChars;
    uniqueChars.insert(lastFour[0]);
    uniqueChars.insert(lastFour[1]);
    uniqueChars.insert(lastFour[2]);
    uniqueChars.insert(lastFour[3]);
    if(uniqueChars.size() == 4)
        return true; // All four chars are unique

    return false;
}

void CSolution<2022, 6>::Execute()
{
    std::ifstream fstream("Day6.txt");
    std::deque<char> lastFour;

    // Find the first location in the stream of 4 unique characters
    std::optional<size_t> start;
    for(size_t pos = 0; !fstream.eof(); ++pos)
    {
        char c;
        fstream >> c;
        lastFour.push_back(c);

        if(pos >= 3)
        {
            if(AreFourUnique(lastFour))
            {
                start = pos + 1;
                break;
            }

            lastFour.pop_front();
        }
    }

    if(start.has_value())
    {
        std::cout << start.value() << std::endl;
    }
}