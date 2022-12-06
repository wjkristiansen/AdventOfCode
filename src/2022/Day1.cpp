#include <iostream>

#include "..\Solutions.h"

void C2022Day1::Execute(const std::string& Name)
{
	std::cout << Name << std::endl;

    ReadData();

    std::cout << "Elf with most calories is [" << m_MaxElf << "] with " << m_MaxCalories << " Calories." << std::endl;
    std::cout << "Sum of top three elves calories is: " << m_Top3[0] + m_Top3[1] + m_Top3[2] << std::endl;
}

void C2022Day1::ReadData()
{
    std::ifstream fstream("Day1.txt");
    std::string line;
    
    for(size_t i = 0;; ++i)
    {
        size_t TotalCalories = 0;

        for(;;)
        {
            std::getline(fstream, line);

            if(line.empty() || fstream.eof())
                break;

            int Calories = std::stoi(line);
            TotalCalories += Calories;
        }

        if (TotalCalories > m_MaxCalories)
        {
            m_MaxCalories = TotalCalories;
            m_MaxElf = i;
        }

        size_t LeastTop3 = 0;

        if (m_Top3[0] < m_Top3[1])
        {
            if (m_Top3[0] < m_Top3[2])
                LeastTop3 = 0;
            else
                LeastTop3 = 2;
        }
        else
        {
            if (m_Top3[1] < m_Top3[2])
                LeastTop3 = 1;
            else
                LeastTop3 = 2;
        }

        if (TotalCalories > m_Top3[LeastTop3])
            m_Top3[LeastTop3] = TotalCalories;

        if (fstream.eof())
            break;
    }
}