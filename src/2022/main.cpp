#include <InCommand.h>
#include <iostream>

#include "Solution.h"

// ------------------------------------------------------------------------------------------------
class MySolution : public CSolutionBase
{
public:
    virtual void Execute()
    {
        std::cout << "MySolution";
    }
};

// ------------------------------------------------------------------------------------------------
class MySolution2 : public CSolutionBase
{
public:
    virtual void Execute()
    {
        std::cout << "MySolution2";
    }
};

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

    Factory.DeclareSolution<MySolution>(2022, 1);
    Factory.DeclareSolution<MySolution2>(2022, 2);

    Factory.ExecuteSolution(Year.Value(), Day.Value());

    return 0;
}