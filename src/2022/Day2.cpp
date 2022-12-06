#include <fstream>
#include <iostream>
#include <string>

#include "..\SolutionFactory.h"

enum class Shape {Rock, Paper, Scissors};
enum class Result {Win, Lose, Draw};

static int ShapeValue(Shape shape)
{
    switch (shape)
    {
    case Shape::Rock:
        return 1;
    case Shape::Paper:
        return 2;
    case Shape::Scissors:
        return 3;
    }

    return 0;
}

static int ResultValue(Result result)
{
    switch (result)
    {
    case Result::Win:
        return 6;
    case Result::Lose:
        return 0;
    case Result::Draw:
        return 3;
    }

    return 0;
}

[[maybe_unused]] static int DoRound(char Opponent, char Self )
{
    int Score = 0;

    switch(Opponent)
    {
    case 'A': // Rock
        switch(Self)
        {
        case 'X': // Rock
            Score = ResultValue(Result::Draw) + ShapeValue(Shape::Rock);
            break;
        case 'Y': // Paper
            Score = ResultValue(Result::Win) + ShapeValue(Shape::Paper);
            break;
        case 'Z': // Scissor
            Score = ResultValue(Result::Lose) + ShapeValue(Shape::Scissors);
            break;
        }
        break;
    case 'B': // Paper
        switch(Self)
        {
        case 'X': // Rock
            Score = ResultValue(Result::Lose) + ShapeValue(Shape::Rock);
            break;
        case 'Y': // Paper
            Score = ResultValue(Result::Draw) + ShapeValue(Shape::Paper);
            break;
        case 'Z': // Scissor
            Score = ResultValue(Result::Win) + ShapeValue(Shape::Scissors);
            break;
        }
        break;
    case 'C': // Scissor
        switch(Self)
        {
        case 'X': // Rock
            Score = ResultValue(Result::Win) + ShapeValue(Shape::Rock);
            break;
        case 'Y': // Paper
            Score = ResultValue(Result::Lose) + ShapeValue(Shape::Paper);
            break;
        case 'Z': // Scissor
            Score = ResultValue(Result::Draw) + ShapeValue(Shape::Scissors);
            break;
        }
        break;
    }

    return Score;
}

static int DoRound2(char Opponent, char Outcome)
{
    int Score = 0;

    switch (Opponent)
    {
    case 'A': // Rock
        switch (Outcome)
        {
        case 'X': // Lose
            Score = ShapeValue(Shape::Scissors) + ResultValue(Result::Lose);
            break;
        case 'Y': // Draw
            Score = ShapeValue(Shape::Rock) + ResultValue(Result::Draw);
            break;
        case 'Z': // Win
            Score = ShapeValue(Shape::Paper) + ResultValue(Result::Win);
            break;
        }
        break;
    case 'B': // Paper
        switch (Outcome)
        {
        case 'X': // Lose
            Score = ShapeValue(Shape::Rock) + ResultValue(Result::Lose);
            break;
        case 'Y': // Draw
            Score = ShapeValue(Shape::Paper) + ResultValue(Result::Draw);
            break;
        case 'Z': // Win
            Score = ShapeValue(Shape::Scissors) + ResultValue(Result::Win);
            break;
        }
        break;
    case 'C': // Scissor
        switch (Outcome)
        {
        case 'X': // Lose
            Score = ShapeValue(Shape::Paper) + ResultValue(Result::Lose);
            break;
        case 'Y': // Draw
            Score = ShapeValue(Shape::Scissors) + ResultValue(Result::Draw);
            break;
        case 'Z': // Win
            Score = ShapeValue(Shape::Rock) + ResultValue(Result::Win);
            break;
        }
        break;
    }

    return Score;
}

void CSolution<2022, 2>::Execute()
{
    std::ifstream fstream("Day2.txt");

    size_t TotalScore = 0;

    for(;;)
    {
        std::string line;
        std::getline(fstream, line);

        if(line.empty() || fstream.eof())
            break;

        auto Score = DoRound2(line[0], line[2]);
        TotalScore += Score;
    }

    std::cout << "Total Score: " << TotalScore << std::endl;
}
