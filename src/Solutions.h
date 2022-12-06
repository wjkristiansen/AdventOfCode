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

    int DoRound(char Opponent, char Self );
    int WinScore(char Self);
    int LoseScore(char Self);
    int DrawScore(char Self);
};