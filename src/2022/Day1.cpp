#include <iostream>

#include "..\Solutions.h"

void C2022Day1::Execute(const std::string& Name)
{
	std::cout << Name << std::endl;

    ReadData();

    int Max = 0;
    size_t Index = 0;
    for (size_t i = 0; i < m_ElfCalories.size(); ++i)
    {
        if (m_ElfCalories[i] > Max)
        {
            Max = m_ElfCalories[i];
            Index = i;
        }
    }

    std::cout << "Elf with most calories is [" << Index << "] with " << Max << " Calories." << std::endl;
}

void C2022Day1::ReadData()
{
    std::ifstream fstream("Day1.txt");
    std::string line;
    
    for(;;)
    {
        m_ElfCalories.emplace_back(0);

        for(;;)
        {
            std::getline(fstream, line);

            if(line.empty() || fstream.eof())
                break;

            int Calories = std::stoi(line);
            m_ElfCalories.back() += Calories;
        }

        if (fstream.eof())
            break;
    }
}