#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "SolutionFactory.h"

class C2022Day1 : public CSolutionBase
{
    std::vector<int> m_ElfCalories;
    size_t m_MaxCalories = 0;
    size_t m_MaxElf = 0;

    size_t m_Top3[3] = {};

public:
    virtual void Execute(const std::string& Name);

    void ReadData();
};

class C2022Day2 : public CSolutionBase
{
public:
    virtual void Execute(const std::string& Name);

    enum class Shape { Rock, Paper, Scissors };
    enum class Result { Win, Lose, Draw };

    static int ShapeValue(Shape shape);
    static int ResultValue(Result result);

    static int DoRound(char Opponent, char Self);
    static int DoRound2(char Opponent, char Outcome );
};

class C2022Day3 : public CSolutionBase
{
    virtual void Execute(const std::string& Name);
};