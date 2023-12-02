#include "pch.h"
#include "SolutionFactory.h"

void CSolution<4>::Execute(int)
{
    std::ifstream fstream("Day4.txt");

    size_t OverlappingCount = 0;

    for(;!fstream.eof();)
    {
        std::string line;
        std::getline(fstream, line);
        if(line.empty())
            break;

        std::pair<int, int> r0(0,0);
        std::pair<int, int> r1(0,0);

        size_t begin = 0;
        size_t end = line.find_first_of('-');
        r0.first = std::stoi(line.substr(begin, end));
        begin = end + 1;
        end = line.find_first_of(',', begin);
        r0.second = std::stoi(line.substr(begin, end));
        begin = end + 1;
        end = line.find_first_of('-', begin);
        r1.first = std::stoi(line.substr(begin, end));
        begin = end + 1;
        r1.second = std::stoi(line.substr(begin)); 

        // std::cout << "[" << r0.first << "," << r0.second << "],[" << r1.first << "," << r1.second << "]" << std::endl;

        // See if r0 overlaps r1
        if(r1.first <= r0.second && r0.first <= r1.second)
            OverlappingCount++;
    }

    std::cout << "Overlapping count: " << OverlappingCount << std::endl;
}
