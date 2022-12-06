#include <InCommand.h>
#include <iostream>

#include "SolutionFactory.h"
#include "Solutions.h"

// ------------------------------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    CSolutionFactory Factory;
    Factory.DeclareSolution<C2022Day1>(2022, 1, "Calorie Counting");
    Factory.DeclareSolution<C2022Day2>(2022, 2, "Rock Paper Scissors");
    Factory.DeclareSolution<C2022Day3>(2022, 3, "Rucksack Reorganization");
    Factory.DeclareSolution<C2022Day4>(2022, 4, "Camp Cleanup");
    int MaxYear;
    int MaxDay;
    Factory.MaxYearDay(MaxYear, MaxDay);

    InCommand::CCommandReader CommandReader("advent", "Advent of Code", argc, argv);
    InCommand::InCommandInt Year(MaxYear);
    InCommand::InCommandInt Day(MaxDay);
    InCommand::InCommandBool ShowHelp;
    CommandReader.DefaultCommandCtx()->DeclareVariableOption(Year, "year", "Advent of Code Year", 'y');
    CommandReader.DefaultCommandCtx()->DeclareVariableOption(Day, "day", "Advent of Code Day", 'y');
    CommandReader.DefaultCommandCtx()->DeclareSwitchOption(ShowHelp, "help", "Get help", 'h');
    CommandReader.ReadOptions();

    Factory.ExecuteSolution(Year.Value(), Day.Value());

    return 0;
}