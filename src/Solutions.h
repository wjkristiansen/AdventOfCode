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