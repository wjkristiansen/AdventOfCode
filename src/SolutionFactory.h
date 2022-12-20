#include <map>
#include <stdexcept>
#include <memory>
#include <iostream>

#pragma once

// ------------------------------------------------------------------------------------------------
struct Date
{
    int Year;
    int Day;

    bool operator<(const Date &o) const
    {
        if(Year < o.Year)
            return true;
        
        if(o.Year < Year)
            return false;

        return Day < o.Day;
    }
};

// ------------------------------------------------------------------------------------------------
template<int Year, int Day>
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
    std::map<Date, SolutionDesc> m_SolutionMap;
    Date m_MaxDate = {};

public:
    template<int Year, int Day>
    void DeclareSolution(const std::string &Name)
    {
        auto [_, Success] = m_SolutionMap.emplace(Date{Year, Day}, SolutionDesc{Name, CSolution<Year, Day>::Execute});
        if(!Success)
        {
            throw(std::runtime_error("Solution for given year/day pair has already been declared."));
        }

        m_MaxDate = std::max(Date{ Year, Day }, m_MaxDate);
    }

    void ExecuteSolution(int Year, int Day, int Part) const
    {
        auto solutionIt = m_SolutionMap.find(Date{Year, Day});
        if(solutionIt == m_SolutionMap.end())
            throw(std::runtime_error("No solution declared for year/day pair."));

        std::cout << "Advent of Code " << Year << " Day " << Day << ": " << solutionIt->second.Name << std::endl;

        // Execute the solution
        auto FnExecute = solutionIt->second.FnExecute;
        FnExecute(Part);
    }

    void MaxDate(int &Year, int &Day) { Year = m_MaxDate.Year; Day = m_MaxDate.Day; }
};