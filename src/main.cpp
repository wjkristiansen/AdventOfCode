#include <InCommand.h>
#include <iostream>

#include "SolutionFactory.h"
#include "Solutions.h"

// ------------------------------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    InCommand::CCommandReader CommandReader("advent", "Advent of Code", argc, argv);
    InCommand::InCommandInt Year(2022);
    InCommand::InCommandInt Day(1);
    InCommand::InCommandBool ShowHelp;
    CommandReader.DefaultCommandCtx()->DeclareVariableOption(Year, "year", "Advent of Code Year", 'y');
    CommandReader.DefaultCommandCtx()->DeclareVariableOption(Day, "day", "Advent of Code Day", 'y');
    CommandReader.DefaultCommandCtx()->DeclareSwitchOption(ShowHelp, "help", "Get help", 'h');
    CommandReader.ReadOptions();

    CSolutionFactory Factory;

    Factory.DeclareSolution<C2022Day1>(2022, 1, "Calorie Counting");

    Factory.ExecuteSolution(Year.Value(), Day.Value());

    return 0;
}