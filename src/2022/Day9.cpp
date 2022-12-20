#include "../pch.h"
#include "../SolutionFactory.h"

template<>
struct std::hash<std::pair<int, int>>
{
    size_t operator ()(const std::pair<int, int> &value) const
    {
        return value.first ^ (value.second << 16);
    }
};

void CSolution<2022, 9>::Execute(int)
{
    std::ifstream fstream("Day9.txt");
    std::unordered_set<std::pair<int, int>> TailPosRecord;

    const size_t NumKnots = 10;
    std::vector<std::pair<int, int>> Knots(NumKnots, {0, 0});
    TailPosRecord.insert({0, 0}); // Record the current tail position

    auto MoveTail = [](const std::pair<int, int> &HeadPos, std::pair<int, int> &TailPos)
    {
        // Adjust the tail position
        int dx = HeadPos.first - TailPos.first;
        int dy = HeadPos.second - TailPos.second;

        if (dy >= -1 && dy <= 1)
        {
            assert(dx >= -2);
            assert(dx <= 2);

            if (dx == 2)
            {
                TailPos.first = HeadPos.first - 1;
                TailPos.second = HeadPos.second;
            }
            else if (dx == -2)
            {
                TailPos.first = HeadPos.first + 1;
                TailPos.second = HeadPos.second;
            }
        }
        else if (dx >= -1 && dx <= 1)
        {
            assert(dy >= -2);
            assert(dy <= 2);

            if (dy == 2)
            {
                TailPos.first = HeadPos.first;
                TailPos.second = HeadPos.second - 1;
            }
            else if (dy == -2)
            {
                TailPos.first = HeadPos.first;
                TailPos.second = HeadPos.second + 1;
            }
        }
        else
        {
            // Not same row or column, so move by same delta
            TailPos.first += dx > 0 ? 1 : -1;
            TailPos.second += dy > 0 ? 1 : -1;
        }
    };

    // Read the grid
    for(; !fstream.eof();)
    {
        char Dir;
        int Dist;
        fstream >> Dir >> Dist;
        for(int i = 0; i < Dist; ++i)
        {
            // Move the head by one
            switch (Dir)
            {
            case 'U':
                Knots[0].second += 1;
                break;
            case 'D':
                Knots[0].second -= 1;
                break;
            case 'R':
                Knots[0].first += 1;
                break;
            case 'L':
                Knots[0].first -= 1;
                break;
            }

            // Update the trailing knots
            for(int j = 1; j < NumKnots; ++j)
            {
                MoveTail(Knots[j - 1], Knots[j]);
            }

            // Record the tail position
            TailPosRecord.insert(Knots[NumKnots - 1]);
        }
    }

    for (int j = 40; j > -30; )
    {
        --j;

        for (int i = -40; i < 20; ++i )
        {
            auto it = TailPosRecord.find(std::pair<int, int>({ i, j }));
            if (it == TailPosRecord.end())
                std::cout << '.';
            else
                std::cout << '#';
        }
        std::cout << std::endl;
    }

    std::cout << "Tail Position Count: " << TailPosRecord.size() << std::endl;
}
