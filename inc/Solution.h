#include <map>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <chrono>

#pragma once

// ------------------------------------------------------------------------------------------------
class CSolution
{
    CSolution() = delete;

public:
    virtual void Execute(int Part) = 0;
};

// ------------------------------------------------------------------------------------------------
class CSolutionFactory
{
    struct SolutionDesc
    {
        int Day;
        std::string Name;
        CSolution *(*FnAllocate)();
        void (*FnDelete)(CSolution *);
    };

    std::map<int, SolutionDesc> m_SolutionMap;
    static std::unique_ptr<CSolutionFactory> m_pSingleton;

public:
    static CSolutionFactory *GetFactory()
    {
        if(!m_pSingleton.get())
        {
            m_pSingleton = std::make_unique<CSolutionFactory>();
        }

        return m_pSingleton.get();
    }

    template<class _Class>
    struct SolutionDecl
    {
        SolutionDecl(int day, const str::string &name)
        {
            CSolutionFactory::GetFactory()->AddSolutionDecl(*this);
        }
    };

    template<class _Class>
    void AddSolutionDecl(const SolutionDecl<_Class> &decl)
    {
        auto [_, Success] = pFactory->m_SolutionMap.emplace(Day, SolutionDesc{
            .Day = Day,
            .Name = Name,
            .fnAllocate = [](){ return new _Class; },
            .fnDelete = [](CSolution *pSolution){ delete pSolution; }});
        if(!Success)
        {
            throw(std::runtime_error("Solution for given day has already been declared."));
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

        if(Part == 0)
            std::cout << "Day " << Day << ": " << solutionIt->second.Name << std::endl;
        else
            std::cout << "Day " << Day << ", Part " << Part << ": " << solutionIt->second.Name << std::endl;

        // Allocate the solution object
        auto fnAllocate = solutionIt->second.fnAllocate;
        auto fnDelete = solutionIt->second.fnDelete;
        CSolution *pSolution = fnAllocate();

        if(pSolution)
        {
            std::chrono::high_resolution_clock clock;
            auto t0 = clock.now();
            pSolution->Execute(Part);
            auto t1 = clock.now();
            auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
            std::cout << std::endl;
            std::cout << "Elapsed time in microseconds: " << microseconds.count() << std::endl;
            fnDelete(pSolution);
        }
    }

    int MaxDay() const { return m_SolutionMap.rbegin()->first;}
};
