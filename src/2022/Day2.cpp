#pragma once

#include "..\Solutions.h"

void C2022Day2::Execute(const std::string &Name)
{
	std::cout << Name << std::endl;

    std::ifstream fstream("Day2.txt");

    size_t TotalScore = 0;

    for(;;)
    {
        std::string line;
        std::getline(fstream, line);

        if(line.empty() || fstream.eof())
            break;

        auto Score = DoRound(line[0], line[2]);
        TotalScore += Score;
    }

    std::cout << "Total Score: " << TotalScore << std::endl;
}

int C2022Day2::DoRound(char Opponent, char Self )
{
    int Score = 0;

    switch(Opponent)
    {
    case 'A': // Rock
        switch(Self)
        {
        case 'X': // Rock
            Score = DrawScore(Self);
            break;
        case 'Y': // Paper
            Score = WinScore(Self);
            break;
        case 'Z': // Scissor
            Score = LoseScore(Self);
            break;
        }
        break;
    case 'B': // Paper
        switch(Self)
        {
        case 'X': // Rock
            Score = LoseScore(Self);
            break;
        case 'Y': // Paper
            Score = DrawScore(Self);
            break;
        case 'Z': // Scissor
            Score = WinScore(Self);
            break;
        }
        break;
    case 'C': // Scissor
        switch(Self)
        {
        case 'X': // Rock
            Score = WinScore(Self);
            break;
        case 'Y': // Paper
            Score = LoseScore(Self);
            break;
        case 'Z': // Scissor
            Score = DrawScore(Self);
            break;
        }
        break;
    }

    return Score;
}

int C2022Day2::WinScore(char Self)
{
    switch(Self)
    {
        case 'X':
            return 1 + 6;
        case 'Y':
            return 2 + 6;
        case 'Z':
            return 3 + 6;
    }

    return 0;
}

int C2022Day2::LoseScore(char Self)
{
    switch(Self)
    {
        case 'X':
            return 1 + 0;
        case 'Y':
            return 2 + 0;
        case 'Z':
            return 3 + 0;
    }

    return 0;
}

int C2022Day2::DrawScore(char Self)
{
    switch(Self)
    {
        case 'X':
            return 1 + 3;
        case 'Y':
            return 2 + 3;
        case 'Z':
            return 3 + 3;
    }

    return 0;
}
