#include "pch.h"
#include "SolutionFactory.h"

static const char *TestInitStacks[] =
{
    "ZN",
    "MCD",
    "P"
};

// [P]     [L]         [T]            
// [L]     [M] [G]     [G]     [S]    
// [M]     [Q] [W]     [H] [R] [G]    
// [N]     [F] [M]     [D] [V] [R] [N]
// [W]     [G] [Q] [P] [J] [F] [M] [C]
// [V] [H] [B] [F] [H] [M] [B] [H] [B]
// [B] [Q] [D] [T] [T] [B] [N] [L] [D]
// [H] [M] [N] [Z] [M] [C] [M] [P] [P]
//  1   2   3   4   5   6   7   8   9 
static const char *InitStacks[] =
{
    "HBVWNMLP",
    "MQH",
    "NDBGFQML",
    "ZTFQMWG",
    "MTHP",
    "CBMJDHGT",
    "MNBFVR",
    "PLHMRGS",
    "PDBCN"
};

class CSolution5: public CSolutionBase
{
    void Execute(int)
    {
        std::ifstream fstream("2022/Day5.txt"); 

        std::vector<std::vector<char>> Stacks;
        for (const char* crates : InitStacks)
        {
            Stacks.emplace_back();
            auto &stack = Stacks.back();

            for (const char* c = crates; *c; ++c)
            {
                stack.push_back(*c);
            }
        }

        for(;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);

            std::istringstream ss(line);
            std::string word;
            size_t count;
            size_t source;
            size_t dest;
            ss >> word;
            ss >> count;
            ss >> word;
            ss >> source;
            ss >> word;
            ss >> dest;
            std::cout << count << ", " << source << ", " << dest << std::endl;
            source--;
            dest--;
            auto& SourceStack = Stacks[source];
            auto& DestStack = Stacks[dest];
            DestStack.insert(DestStack.end(), SourceStack.end() - count, SourceStack.end());
            SourceStack.erase(SourceStack.end() - count, SourceStack.end());
        }

        for (auto& stack : Stacks)
        {
            std::cout << stack.back();
        }
    }
};

DECLARE_SOLUTION(CSolution5, 5, "Supply Stacks");
