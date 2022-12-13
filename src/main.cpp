#include <InCommand.h>
#include <iostream>

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

    int MaxYear;
    int MaxDay;
    Factory.MaxDate(MaxYear, MaxDay);

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