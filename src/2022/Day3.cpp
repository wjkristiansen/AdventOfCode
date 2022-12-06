#include "..\Solutions.h"

constexpr size_t Priority(char c)
{
    if(c >= 'a' && c <= 'z')
        return 1 + size_t(c - 'a');
    
    if(c >= 'A' && c <= 'Z')
        return 27 + size_t(c - 'A');
    
    return 0;
}

void C2022Day3::Execute(const std::string &Name)
{
    std::cout << Name << std::endl;

    std::ifstream fstream("Day3.txt");

    size_t SumOfPriorities = 0;

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

    std::cout << "Sum of priorities = " << SumOfPriorities << std::endl;
}