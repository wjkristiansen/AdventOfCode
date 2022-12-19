#include "../pch.h"
#include "../SolutionFactory.h"

static constexpr size_t Priority(char c)
{
    if(c >= 'a' && c <= 'z')
        return 1 + size_t(c - 'a');
    
    if(c >= 'A' && c <= 'Z')
        return 27 + size_t(c - 'A');
    
    return 0;
}

void CSolution<2022, 3>::Execute()
{
    std::ifstream fstream("Day3.txt");

    size_t SumOfPriorities = 0;

#if 0
    for(;!fstream.eof();)
    {
        std::string line;
        std::getline(fstream, line);
        if(line.empty())
            break;

        size_t NumItems = line.length();

        // Find the one item that is in both compartments
        for (size_t i = 0; i < NumItems / 2; ++i)
        {
            char c = line[i];
            auto res = line.find(c, NumItems / 2);
            if (res != std::string::npos)
            {
                auto P = Priority(c);
                SumOfPriorities += P;
                break;
            }
        }
    }
#else
    for (; !fstream.eof();)
    {
        std::vector<int> elfMasks(52, 0);

        for (int mask = 1; mask < (1 << 3); mask = mask << 1)
        {
            std::string line;
            std::getline(fstream, line);

            for (auto c : line)
            {
                auto p = Priority(c);
                elfMasks[p - 1] |= mask;

                if (elfMasks[p - 1] == 0x7)
                {
                    SumOfPriorities += p;
                    break;
                }
            }
        }
    }
#endif

    std::cout << "Sum of priorities = " << SumOfPriorities << std::endl;
}