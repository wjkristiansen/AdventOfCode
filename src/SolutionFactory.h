#include <map>
#include <stdexcept>
#include <memory>

#pragma once

// ------------------------------------------------------------------------------------------------
struct YearDay
{
    int Year;
    int Day;

    bool operator<(const YearDay &o) const
    {
        if(Year < o.Year)
            return true;
        
        if(o.Year < Year)
            return false;

        return Day < o.Day;
    }
};

// ------------------------------------------------------------------------------------------------
class CSolutionBase
{
public:
    CSolutionBase() = default;
    virtual ~CSolutionBase() = default;
    virtual void Execute(const std::string &Name) = 0;
};

// ------------------------------------------------------------------------------------------------
template<class Base>
class CSolutionClass : public Base
{
    YearDay m_MaxYearDay;
    std::string m_Name;

public:
    static std::unique_ptr<CSolutionBase> Create()
    {
        return std::make_unique<CSolutionClass<Base>>();
    }
};

// ------------------------------------------------------------------------------------------------
struct SolutionDesc
{
    std::string Name;
    std::unique_ptr<CSolutionBase>(*FNCreate)();
};

// ------------------------------------------------------------------------------------------------
class CSolutionFactory
{
    std::map<YearDay, SolutionDesc> m_SolutionMap;
    YearDay m_MaxYearDay = {};

public:
    template<class SolutionClass>
    void DeclareSolution(int Year, int Day, const std::string &Name)
    {
        YearDay yearDay{ Year, Day };
        auto [_, Success] = m_SolutionMap.emplace(yearDay, SolutionDesc{ Name, CSolutionClass<SolutionClass>::Create } );
        if(!Success)
        {
            throw(std::runtime_error("Solution for given year/day pair has already been declared."));
        }

        m_MaxYearDay = std::max(yearDay, m_MaxYearDay);
    }

    void ExecuteSolution(int Year, int Day) const
    {
        auto solutionIt = m_SolutionMap.find(YearDay{Year, Day});
        if(solutionIt == m_SolutionMap.end())
            throw(std::runtime_error("No solution declared for year/day pair."));

        // Execute the solution
        auto FnCreate = solutionIt->second.FNCreate;
        std::unique_ptr<CSolutionBase> pSolution = FnCreate();
        pSolution->Execute(solutionIt->second.Name);
    }

    void MaxYearDay(int &Year, int &Day) { Year = m_MaxYearDay.Year; Day = m_MaxYearDay.Day; }
};