#include "pch.h"
#include "SolutionFactory.h"

class CSolution3: public CSolutionBase
{
public:
    CSolution3() : CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        int minPart = part > 0 ? part : 1;
        int maxPart = part > 0 ? part : 2;
        for (int runPart = minPart; runPart <= maxPart; ++runPart)
        {
            if (runPart == 1)
            {
                std::ifstream fstream("2024/Day3.txt");
                std::ostringstream fcontent;
                fcontent << fstream.rdbuf();
                std::string content = fcontent.str();
                std::string pattern = R"(mul\(([0-9]+),([0-9]+)\))";
                std::regex re(pattern, std::regex_constants::ECMAScript);
                std::smatch matches;
                auto searchStart = content.cbegin();

                uint32_t accum = 0;
                while(std::regex_search(searchStart, content.cend(), matches, re))
                {
                    std::string sval1 = matches[1];
                    std::string sval2 = matches[2];
                    uint32_t v1 = std::stoul(sval1);
                    uint32_t v2 = std::stoul(sval2);
                    accum += v1 * v2;
                    searchStart = matches.suffix().first;
                }

                std::cout << "Part 1: Sum of products: " << accum << std::endl;
            }
            else
            {
                std::ifstream fstream("2024/Day3.txt");
                std::ostringstream fcontent;
                fcontent << fstream.rdbuf();
                std::string content = fcontent.str();
                std::string pattern = R"(mul\(([0-9]+),([0-9]+)\)|do\(\)|don't\(\))";
                std::regex re(pattern, std::regex_constants::ECMAScript);
                std::smatch matches;
                auto searchStart = content.cbegin();

                uint32_t accum = 0;
                bool enabled = true;
                while (std::regex_search(searchStart, content.cend(), matches, re))
                {
                    std::string inst = matches[0];
                    if (inst == std::string("do()"))
                    {
                        enabled = true;
                    }
                    else if (inst == std::string("don't()"))
                    {
                        enabled = false;
                    }
                    else
                    {
                        if (enabled)
                        {
                            std::string sval1 = matches[1];
                            std::string sval2 = matches[2];
                            uint32_t v1 = std::stoul(sval1);
                            uint32_t v2 = std::stoul(sval2);
                            accum += v1 * v2;
                        }
                    }
                    searchStart = matches.suffix().first;
                }

                std::cout << "Part 2: Sum of products: " << accum << std::endl;
            }
        }
    }
};

DECLARE_SOLUTION(CSolution3, 3, "Mull It Over");
