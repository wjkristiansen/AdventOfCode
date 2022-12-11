#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <assert.h>

#include "..\SolutionFactory.h"

template<>
struct std::hash<std::pair<int, int>>
{
    size_t operator ()(const std::pair<int, int> &value) const
    {
        return value.first ^ (value.second << 16);
    }
};

void CSolution<2022, 9>::Execute()
{
    std::ifstream fstream("Day9.txt");
    std::unordered_set<std::pair<int, int>> TailPosRecord;

    std::pair<int, int> HeadPos{ 0, 0 };
    std::pair<int, int> TailPos{ 0, 0 };
    TailPosRecord.insert(TailPos);

    auto MoveHead = [&HeadPos, &TailPos, &TailPosRecord](char Dir, int Dist)
    {
        for (int i = 0; i < Dist; ++i)
        {
            // Move the head by one
            switch (Dir)
            {
            case 'U':
                HeadPos.second -= 1;
                break;
            case 'D':
                HeadPos.second += 1;
                break;
            case 'L':
                HeadPos.first -= 1;
                break;
            case 'R':
                HeadPos.first += 1;
                break;
            }

            // Adjust the tail position
            int dx = HeadPos.first - TailPos.first;
            int dy = HeadPos.second - TailPos.second;
            if (dx > 1)
            {
                TailPos.first = HeadPos.first - 1;
                TailPos.second = HeadPos.second;
            }
            else if (dx < -1)
            {
                TailPos.first = HeadPos.first + 1;
                TailPos.second = HeadPos.second;
            }
            else if (dy > 1)
            {
                TailPos.first = HeadPos.first;
                TailPos.second = HeadPos.second - 1;
            }
            else if (dy < -1)
            {
                TailPos.first = HeadPos.first;
                TailPos.second = HeadPos.second + 1;
            }

            // Record the tail position
            TailPosRecord.insert(TailPos);
        }
    };

    // Read the grid
    for(; !fstream.eof();)
    {
        char Dir;
        int Dist;
        fstream >> Dir >> Dist;
        MoveHead(Dir, Dist);
    }

    std::cout << "Tail Position Count: " << TailPosRecord.size() << std::endl;
}
