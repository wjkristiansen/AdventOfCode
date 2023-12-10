#include "pch.h"
#include "SolutionFactory.h"

class CSolution8 : public CSolutionBase
{
public:
    CSolution8() : CSolutionBase(8, "Haunted Wasteland")
    {
    }

    template<typename _T>
    static _T LeastCommonMultiple(std::vector<_T> const& numbers)
    {
        return std::accumulate(numbers.begin(), numbers.end(), (_T)1, std::lcm<_T, _T>);
    }

    virtual void Execute(int part)
    {
        std::ifstream fstream("2023/Day8.txt");
        std::string line;

        std::string pattern = R"(([0-9A-Z]{3})\s*=\s*\(\s*([0-9A-Z]{3})\s*,\s*([0-9A-Z]{3})\s*\))";
        std::regex re(pattern, std::regex_constants::ECMAScript);
        std::smatch matches;

        // Get the LR instructions
        std::string instructions;
        std::getline(fstream, instructions);

        struct Node
        {
            Node(const std::string& name, const std::string& left, const std::string& right) :
                Name(name),
                Left(left),
                Right(right) {}
            std::string Name;
            std::string Left;
            std::string Right;

            std::map<std::string, Node>::iterator LeftIt;
            std::map<std::string, Node>::iterator RightIt;
            std::vector<size_t> DistancesToEnd;
        };

        std::map<std::string, Node> nodeMap;
        std::vector<std::map<std::string, Node>::iterator> startNodeIterators;

        // Read the network node definitions
        for (; !fstream.eof();)
        {
            std::getline(fstream, line);
            if (line.empty())
                continue;

            if (std::regex_search(line, matches, re))
            {
                // Add a node to the node map
                auto [it, _] = nodeMap.emplace(matches[1], std::move(Node(matches[1], matches[2], matches[3])));

                if (matches[1].str().ends_with('A'))
                {
                    startNodeIterators.push_back(it);
                }
            }
        }

        // Fixup node left/right iterators in the node map
        for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it)
        {
            it->second.LeftIt = nodeMap.find(it->second.Left);
            it->second.RightIt = nodeMap.find(it->second.Right);
            it->second.DistancesToEnd.resize(instructions.size());
        }

        std::vector<size_t> allSteps;
        for (auto it : startNodeIterators)
        {
            size_t steps = 0;
            int instrLength = (int)instructions.length();
            for (; !it->second.Name.ends_with('Z');)
            {
                int instrIndex = steps % instrLength;
                assert(it != nodeMap.end());
                if (instructions[instrIndex] == 'L')
                {
                    it = it->second.LeftIt;
                }
                else
                {
                    it = it->second.RightIt;
                }
                steps++;
            }

            allSteps.push_back(steps);
        }

        size_t lcm = LeastCommonMultiple(allSteps);

        std::cout << lcm << std::endl;
    }
};

static CSolution8 Solution;