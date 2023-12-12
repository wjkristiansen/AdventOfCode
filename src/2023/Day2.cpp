#include "pch.h"
#include "SolutionFactory.h"

class CSolution2: public CSolutionBase
{
public:
    CSolution2() : CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        std::ifstream fstream("2023/Day2p1.txt");
        std::string line;

        const int maxRed = 12;
        const int maxGreen = 13;
        const int maxBlue = 14;

        int possibleGamesSum = 0;
        int sumPower = 0;

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

            int minRed = 0;
            int minGreen = 0;
            int minBlue = 0;
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

                    if (color == "red")
                    {
                        if(count > maxRed)
                            isPossible = false;

                        if (count > minRed)
                            minRed = int(count);
                    }

                    if (color == "green")
                    {
                        if(count > maxGreen)
                            isPossible = false;

                        if (count > minGreen)
                            minGreen = int(count);
                    }

                    if(color == "blue")
                    {
                        if(count > maxBlue)
                            isPossible = false;

                        if (count > minBlue)
                            minBlue = int(count);
                    }
                }
            }

            int power = minRed * minGreen * minBlue;
            sumPower += power;

            if (isPossible)
            {
                possibleGamesSum += int(gameId);
            }
        }

        std::cout << "Part 1: Sum of possible game Ids: " << possibleGamesSum << std::endl;
        std::cout << "Part 2: Sum of powers: " << sumPower << std::endl;
    }
};

DECLARE_SOLUTION(CSolution2, 2, "Cube Conundrum");
