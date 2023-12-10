#include <map>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <chrono>

#pragma once

class CSolutionBase
{
    std::string m_Title;

public:
    CSolutionBase(int Day, const std::string& name);
    
    const char* GetTitle() const { return m_Title.c_str(); }

    virtual void Execute(int part) = 0;
};

// ------------------------------------------------------------------------------------------------
class CSolutionFactory
{
    std::map<int, CSolutionBase *> m_SolutionMap;
    static std::unique_ptr<CSolutionFactory> m_pSingleton;

public:
    CSolutionFactory() = default;

    static CSolutionFactory *GetFactory()
    {
        if (m_pSingleton.get() == nullptr)
        {
            m_pSingleton = std::make_unique<CSolutionFactory>();
        }

        return m_pSingleton.get();
    }

    void DeclareSolution(int day, CSolutionBase *pSolution)
    {
        auto [_, Success] = m_SolutionMap.emplace(day, pSolution);
        if(!Success)
        {
            throw(std::runtime_error("Solution for given day pair has already been declared."));
        }
    }

    void ExecuteSolution(int Day, int Part)
    {
        if(Day == 0)
        {
            // Default to max declared day
            Day = MaxDay();
        }

        auto solutionIt = m_SolutionMap.find(Day);
        if(solutionIt == m_SolutionMap.end())
            throw(std::runtime_error("No solution declared for year/day pair."));

        CSolutionBase* pSolution = solutionIt->second;

        if(Part == 0)
            std::cout << "Day " << Day << ": " << pSolution->GetTitle() << std::endl;
        else
            std::cout << "Day " << Day << ", Part " << Part << ": " << pSolution->GetTitle() << std::endl;

        std::chrono::high_resolution_clock clock;
        auto t0 = clock.now();
        pSolution->Execute(Part);
        auto t1 = clock.now();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
        std::cout << std::endl;
        std::cout << "Elapsed time in microseconds: " << microseconds << std::endl;
    }

    int MaxDay() const { return m_SolutionMap.rbegin()->first;}
};

inline CSolutionBase::CSolutionBase(int Day, const std::string& name):
    m_Title(name)
{
    CSolutionFactory *pFactory = CSolutionFactory::GetFactory();
    pFactory->DeclareSolution(Day, this);
}
