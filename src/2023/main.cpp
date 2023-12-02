#include "pch.h"
#include "SolutionFactory.h"

#ifdef _WIN32
#define DYNAMIC_EXPORT __declspec(dllexport)
#else
#define DYNAMIC_EXPORT __attribute__((visibility("default")))
#endif

extern "C" DYNAMIC_EXPORT void ExecuteChallenge(int day, int part)
{
    CSolutionFactory Factory;

    std::cout << "Advent of Code 2023" << std::endl;
    
    Factory.ExecuteSolution(day, part);
}