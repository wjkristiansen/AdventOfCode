#include "pch.h"
#include "SolutionFactory.h"

class CSolution1: public CSolutionBase
{
public:
    CSolution1() : CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        if (part == 1)
        {
            std::ifstream fstream("2024/Day1.txt");
            std::string line;
            std::multiset<int> m1;
            std::multiset<int> m2;

            for (;!fstream.eof();)
            {
                std::getline(fstream, line);
                if (line.empty())
                    break;
                std::istringstream ss(line);
                int v1, v2;
                ss >> v1;
                ss >> v2;
                m1.insert(v1);
                m2.insert(v2);
            }

            size_t sumDistance = 0;
            auto it1 = m1.begin();
            auto it2 = m2.begin();
            for (; it1 != m1.end();)
            {
                sumDistance += std::abs(*it2 - *it1);
                it1++;
                it2++;
            }

            std::cout << "Part 1: Sum of distances = " << sumDistance << std::endl;
        }
        else
        {
            std::ifstream fstream("2024/Day1.txt");
            std::string line;
            std::vector<int> leftList;
            std::unordered_map<int, int> rightCounts;

            for (;!fstream.eof();)
            {
                std::getline(fstream, line);
                if (line.empty())
                    break;
                std::istringstream ss(line);
                int v1, v2;
                ss >> v1;
                ss >> v2;
                leftList.push_back(v1);
                auto it = rightCounts.find(v2);
                if (it == rightCounts.end())
                    rightCounts.emplace(v2, 1);
                else
                    it->second += 1;
            }

            uint64_t score = 0;
            for (auto v : leftList)
            {
                auto it = rightCounts.find(v);
                if (it != rightCounts.end())
                    score += v * it->second;
            }

            std::cout << "Part 2: Similarity Score = " << score << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution1, 1, "Historian Hysteria");
