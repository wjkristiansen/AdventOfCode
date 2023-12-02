#include "SolutionFactory.h"

#ifdef _WIN32
#define DYNAMIC_EXPORT __declspec(dllexport)
#else
#define DYNAMIC_EXPORT __attribute__((visibility("default")))
#endif

extern "C" DYNAMIC_EXPORT void ExecuteChallenge(int day, int part)
{
    CSolutionFactory Factory;
    Factory.DeclareSolution<1>("Calorie Counting");
    Factory.DeclareSolution<2>("Rock Paper Scissors");
    Factory.DeclareSolution<3>("Rucksack Reorganization");
    Factory.DeclareSolution<4>("Camp Cleanup");
    Factory.DeclareSolution<5>("Supply Stacks");
    Factory.DeclareSolution<6>("Tuning Trouble");
    Factory.DeclareSolution<7>("No Space Left On Device");
    Factory.DeclareSolution<8>("Treetop Tree House");
    Factory.DeclareSolution<9>("Rope Bridge");
    Factory.DeclareSolution<10>("Cathode-Ray Tube");
    Factory.DeclareSolution<11>("Monkey in the Middle");
    Factory.DeclareSolution<12>("Hill Climbing Algorithm");
    Factory.DeclareSolution<13>("Distress Signal");
    Factory.DeclareSolution<14>("Regolith Reservoir");
    Factory.DeclareSolution<15>("Beacon Exclusion Zone");

    std::cout << "Advent of Code 2022" << std::endl;
    
    Factory.ExecuteSolution(day, part);
}