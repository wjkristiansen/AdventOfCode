#include "pch.h"
#include "SolutionFactory.h"

class CSolution1: public CSolutionBase
{
    void Execute(int)
    {
        std::ifstream fstream("2022/Day1.txt");
        std::string line;

        size_t MaxCalories = 0;
        size_t MaxElf = 0;
        size_t Top3[3] = {};
        
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

            if (TotalCalories > MaxCalories)
            {
                MaxCalories = TotalCalories;
                MaxElf = i;
            }

            size_t LeastTop3 = 0;

            if (Top3[0] < Top3[1])
            {
                if (Top3[0] < Top3[2])
                    LeastTop3 = 0;
                else
                    LeastTop3 = 2;
            }
            else
            {
                if (Top3[1] < Top3[2])
                    LeastTop3 = 1;
                else
                    LeastTop3 = 2;
            }

            if (TotalCalories > Top3[LeastTop3])
                Top3[LeastTop3] = TotalCalories;

            if (fstream.eof())
                break;
        }

        std::cout << "Elf with most calories is [" << MaxElf << "] with " << MaxCalories << " Calories." << std::endl;
        std::cout << "Sum of top three elves calories is: " << Top3[0] + Top3[1] + Top3[2] << std::endl;
    }
};

DECLARE_SOLUTION(CSolution1, 1, "Calorie Counting");