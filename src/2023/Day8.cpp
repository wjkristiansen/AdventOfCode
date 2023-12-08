#include "pch.h"
#include "SolutionFactory.h"

void CSolution<8>::Execute(int part)
{
    std::ifstream fstream("2023/Day8.txt");
    std::string line;

    std::string pattern = R"(([A-Z]{3})\s*=\s*\(\s*([A-Z]{3})\s*,\s*([A-Z]{3})\s*\))";
    std::regex re(pattern, std::regex_constants::ECMAScript);
    std::smatch matches;

    // Get the LR instructions
    std::string instructions;
    std::getline(fstream, instructions);

    std::map<std::string, std::pair<std::string, std::string>> nodeMap;

    // Read the network nodes
    for (; !fstream.eof();)
    {
        std::getline(fstream, line);
        if(line.empty())
            continue;

        if (std::regex_search(line, matches, re))
        {
            // Add a node to the node map
            nodeMap.emplace(matches[1], std::make_pair(matches[2], matches[3]));
        }
    }

    auto itZZZ = nodeMap.find("ZZZ");
    int steps = 0;
    int instrLength = instructions.length();
    for(auto it = nodeMap.find("AAA"); it != itZZZ; )
    {
        int instrIndex = steps % instrLength;
        assert(it != nodeMap.end());
        if (instructions[instrIndex] == 'L')
        {
            it = nodeMap.find(it->second.first);
        }
        else
        {
            it = nodeMap.find(it->second.second);
        }
        steps++;
    }

    std::cout << "Steps: " << steps << std::endl;
}