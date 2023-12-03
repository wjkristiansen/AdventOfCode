#include "pch.h"
#include "SolutionFactory.h"

void CSolution<2>::Execute(int part)
{
    std::ifstream fstream("2023/Day2p1.txt");
    std::string line;

    const int maxRed = 12;
    const int maxGreen = 13;
    const int maxBlue = 14;

    int possibleGamesSum = 0;

    for (;;)
    {
        bool isPossible = true;
        std::getline(fstream, line);
        if (line.empty())
            break;

        std::istringstream ss(line);
        std::string game;
        size_t gameId;
        ss >> game;
        assert(game == std::string("Game"));
        ss >> gameId;
        ss.ignore(2, ' ');

        int turn = 0;
        for (;!ss.eof();)
        {
            std::string turnString;
            std::getline(ss, turnString, ';');
            std::istringstream turnss(turnString);

            for (; !turnss.eof();)
            {
                size_t count;
                std::string color;
                std::string colorCount;
                std::getline(turnss, colorCount, ',');
                std::istringstream countss(colorCount);
                countss >> count;
                countss >> color;

                if (color == "red" && count > maxRed)
                {
                    isPossible = false;
                    break;
                }

                if (color == "green" && count > maxGreen)
                {
                    isPossible = false;
                    break;
                }

                if(color == "blue" && count > maxBlue)
                {
                    isPossible = false;
                    break;
                }
            }

            if (!isPossible)
            {
                break;
            }
        }

        if (isPossible)
        {
            possibleGamesSum += gameId;
        }
    }

    std::cout << "Sum of possible game Ids: " << possibleGamesSum << std::endl;
}
