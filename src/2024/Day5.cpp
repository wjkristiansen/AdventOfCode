#include "pch.h"
#include "SolutionFactory.h"

class CSolution5: public CSolutionBase
{
public:
    CSolution5() : CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        std::ifstream fstream("2024/Day5.txt");
        std::map<int, std::set<int>> rules;
        std::vector<std::vector<int>> updates;

        for (;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);
            if (line.empty())
                break; // skip to updates section
            std::istringstream ss(line);
            int v1, v2;
            ss >> v1;
            ss.ignore(1, '|');
            ss >> v2;
            rules[v1].emplace(v2);
        }

        for (;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);
            if (line.empty())
                break;
            updates.push_back({});
            std::vector<int>& update = updates.back();
            std::istringstream ss(line);
            std::string page;
            while (std::getline(ss, page, ','))
            {
                update.push_back(std::stoi(page));
            }
        }

        auto OutOfOrder = [rules](int v1, int v2)
            {
                auto it = rules.find(v2);

                // If v1 is in the after pages of v2 then
                // the values are out of order
                if (it == rules.end())
                    return false; // No rule for v2

                auto& afterPages = it->second;
                if (afterPages.find(v1) == afterPages.end())
                    return false; // Not in the after pages

                return true;
            };

        for (auto& rule : rules)
        {
            std::cout << rule.first << std::endl;
            std::cout << "  ";
            for (auto value : rule.second)
            {
                std::cout << value << ",";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;

        int sumOfMiddleValues = 0;
        int sumOfSortedMiddleValues = 0;
        for (auto& update : updates)
        {
            size_t middleIndex = update.size() / 2;
            bool InOrder = true;
            auto it = update.begin();
            int last = *it;
            for (++it; it != update.end(); ++it)
            {
                std::cout << last << ",";

                int value = *it;
                if (OutOfOrder(last, value))
                {
                    InOrder = false;
                }
                last = value;
            }

            if (InOrder)
                sumOfMiddleValues += update[middleIndex];
            
            std::cout << last << ": " << (InOrder ? "yes" : "no") << std::endl;

            if (!InOrder)
            {
                std::sort(update.begin(), update.end(), [&](int a, int b) { return !OutOfOrder(a, b); });
                std::cout << "  Sorted: ";
                for (auto v : update)
                {
                    std::cout << v << ",";
                }
                std::cout << std::endl;
                sumOfSortedMiddleValues += update[middleIndex];
            }
        }

        std::cout << "Part 1: Sum of correct middle values: " << sumOfMiddleValues << std::endl;
        std::cout << "Part 2: Sum of fixed middle values: " << sumOfSortedMiddleValues << std::endl;
    }
};

DECLARE_SOLUTION(CSolution5, 5, "Print Queue");
