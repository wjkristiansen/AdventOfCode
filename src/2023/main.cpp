#include "pch.h"
#include "SolutionFactory.h"

#ifdef _WIN32
#define DYNAMIC_EXPORT __declspec(dllexport)
#else
#define DYNAMIC_EXPORT __attribute__((visibility("default")))
#endif

std::unique_ptr<CSolutionFactory> CSolutionFactory::m_pSingleton;

extern "C" DYNAMIC_EXPORT void ListSolutions()
{
    CSolutionFactory* pFactory = CSolutionFactory::GetFactory();

    std::cout << "Advent of Code 2023" << std::endl;
    
    pFactory->ListSolutions();
}

extern "C" DYNAMIC_EXPORT void ExecuteSolution(int day, int part)
{
    CSolutionFactory* pFactory = CSolutionFactory::GetFactory();

    std::cout << "Advent of Code 2023" << std::endl;
    
    pFactory->ExecuteSolution(day, part);
}