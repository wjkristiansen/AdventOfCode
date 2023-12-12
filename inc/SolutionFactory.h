#include <map>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <chrono>

#pragma once

#define DECLARE_SOLUTION(Class, Day, Name) \
    static CSolutionFactory::CSolutionAllocator<Class> SolutionAllocator(Day, Name)

class CSolutionBase
{
public:
    CSolutionBase() = default;

    virtual void Execute(int part) = 0;
};

// ------------------------------------------------------------------------------------------------
class CSolutionFactory
{
    class CSolutionAllocatorBase
    {
        std::string m_Title;
    public:
        CSolutionAllocatorBase(const std::string title):
            m_Title(title) {}
        const std::string& GetTitle() const { return m_Title; }
        virtual CSolutionBase* Allocate() = 0;
        virtual void Deallocate(CSolutionBase* pSolution) = 0;
    };

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

    template<class _T>
    class CSolutionAllocator : public CSolutionAllocatorBase
    {
    public:
        CSolutionAllocator(int day, const std::string &title): CSolutionAllocatorBase(title)
        {
            CSolutionFactory::GetFactory()->DeclareSolution(day, this);
        }
        virtual CSolutionBase* Allocate() final { return new _T(); }
        virtual void Deallocate(CSolutionBase* pSolution) final { delete pSolution; }
    };

    void DeclareSolution(int day, CSolutionAllocatorBase *pAllocator)
    {
        auto [_, Success] = m_SolutionMap.emplace(day, pAllocator);
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

        CSolutionAllocatorBase* pAllocator = solutionIt->second;
        CSolutionBase* pSolution = pAllocator->Allocate();

        if (pSolution)
        {
            if (Part == 0)
                std::cout << "Day " << Day << ": " << pAllocator->GetTitle() << std::endl;
            else
                std::cout << "Day " << Day << ", Part " << Part << ": " << pAllocator->GetTitle() << std::endl;

            std::chrono::high_resolution_clock clock;
            auto t0 = clock.now();
            pSolution->Execute(Part);
            auto t1 = clock.now();
            auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
            std::cout << std::endl;
            std::cout << "Elapsed time in microseconds: " << microseconds << std::endl;

            solutionIt->second->Deallocate(pSolution);
        }
    }

    int MaxDay() const { return m_SolutionMap.rbegin()->first; }

private:
    std::map<int, CSolutionAllocatorBase*> m_SolutionMap;
    static std::unique_ptr<CSolutionFactory> m_pSingleton;   
};
