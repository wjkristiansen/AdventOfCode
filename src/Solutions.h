#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SolutionFactory.h"

class C2022Day1 : public CSolutionBase
{
    std::vector<int> m_ElfCalories;

public:
    virtual void Execute(const std::string& Name);

    void ReadData();
};