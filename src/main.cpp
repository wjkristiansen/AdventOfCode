#include "pch.h"
#include "SolutionFactory.h"

// ------------------------------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    CSolutionFactory Factory;
    Factory.DeclareSolution<2022, 1>("Calorie Counting");
    Factory.DeclareSolution<2022, 2>("Rock Paper Scissors");
    Factory.DeclareSolution<2022, 3>("Rucksack Reorganization");
    Factory.DeclareSolution<2022, 4>("Camp Cleanup");
    Factory.DeclareSolution<2022, 5>("Supply Stacks");
    Factory.DeclareSolution<2022, 6>("Tuning Trouble");
    Factory.DeclareSolution<2022, 7>("No Space Left On Device");
    Factory.DeclareSolution<2022, 8>("Treetop Tree House");
    Factory.DeclareSolution<2022, 9>("Rope Bridge");
    Factory.DeclareSolution<2022, 10>("Cathode-Ray Tube");
    Factory.DeclareSolution<2022, 11>("Monkey in the Middle");
    Factory.DeclareSolution<2022, 12>("Hill Climbing Algorithm");
    Factory.DeclareSolution<2022, 13>("Distress Signal");
    Factory.DeclareSolution<2022, 14>("Regolith Reservoir");
    Factory.DeclareSolution<2022, 15>("Beacon Exclusion Zone");

    int MaxYear;
    int MaxDay;
    Factory.MaxDate(MaxYear, MaxDay);

    InCommand::CCommandReader CommandReader("advent", "Advent of Code");
    InCommand::Int Year(MaxYear);
    InCommand::Int Day(MaxDay);
    InCommand::Int Part(2);
    InCommand::Bool ShowHelp;
    CommandReader.DeclareSwitchOption(Year, "year", "Advent of Code Year", 'y');
    CommandReader.DeclareSwitchOption(Day, "day", "Advent of Code Day", 'd');
    CommandReader.DeclareSwitchOption(Part, "part", InCommand::Domain(1, 2), "Indicates which part of a day's challenge to run", 'p');
    CommandReader.DeclareBoolSwitchOption(ShowHelp, "help", "Get help", 'h');
    CommandReader.ReadArguments(argc, argv);

    Factory.ExecuteSolution(Year.Value(), Day.Value(), Part.Value());

    return 0;
}