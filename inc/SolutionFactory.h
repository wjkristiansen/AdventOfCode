#include <map>
#include <stdexcept>
#include <memory>
#include <iostream>

#pragma once

// ------------------------------------------------------------------------------------------------
template<int Day>
class CSolution
{
public:
    static void Execute(int Part);
};

// ------------------------------------------------------------------------------------------------
struct SolutionDesc
{
    std::string Name;
    void (*FnExecute)(int Part);
};

// ------------------------------------------------------------------------------------------------
class CSolutionFactory
{
    std::map<int, SolutionDesc> m_SolutionMap;

public:
    template<int Day>
    void DeclareSolution(const std::string &Name)
    {
        auto [_, Success] = m_SolutionMap.emplace(Day, SolutionDesc{Name, CSolution<Day>::Execute});
        if(!Success)
        {
            throw(std::runtime_error("Solution for given year/day pair has already been declared."));
        }
    }

    void ExecuteSolution(int Day, int Part) const
    {
        if(Day == 0)
        {
            // Default to max declared day
            Day = MaxDay();
        }

        auto solutionIt = m_SolutionMap.find(Day);
        if(solutionIt == m_SolutionMap.end())
            throw(std::runtime_error("No solution declared for year/day pair."));

        if(Part == 0)
            std::cout << "Day " << Day << ": " << solutionIt->second.Name << std::endl;
        else
            std::cout << "Day " << Day << ", Part " << Part << ": " << solutionIt->second.Name << std::endl;

        // Execute the solution
        auto FnExecute = solutionIt->second.FnExecute;
        FnExecute(Part);
    }

    int MaxDay() const { return m_SolutionMap.rbegin()->first;}
};