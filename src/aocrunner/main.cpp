#include "pch.h"

class Runner
{
private:
#ifdef _WIN32
    HMODULE m_handle = NULL;
#else
    void* m_handle = nullptr;
#endif

public:
    Runner() = default;

    bool Load(int year)
    {
        Unload();

        std::ostringstream filename;
        filename << "aoc" << year;

#ifdef _WIN32
        filename << ".dll";
        m_handle = LoadLibrary(filename.str().c_str());
#else
        filename << ".so";
        m_handle = dlopen(filename.str().c_str(), RTLD_LAZY);
#endif
        if (!m_handle)
            return false;

        return true;
    }

    void Unload()
    {
#ifdef _WIN32
        FreeLibrary(m_handle);
#else
        dlclose(m_handle);
#endif
    }

    int ListSolutions()
    {
        if (!m_handle)
            return -1;

        typedef void (*ListSolutionsFunc)();
#ifdef _WIN32
        ListSolutionsFunc listSolutions = reinterpret_cast<ListSolutionsFunc>(GetProcAddress(m_handle, "ListSolutions"));
#else
        ExecuteSolutionFunc executeChallenge = reinterpret_cast<ExecuteSolutionFunc>(dlsym(handle, "ListSolutions"));
#endif
        if (!listSolutions)
            return -1;

        listSolutions();

        return 0;
    }

    int Execute(int day, int part) const
    {
        if (!m_handle)
            return -1;

        typedef void (*ExecuteSolutionFunc)(int day, int part);
#ifdef _WIN32
        ExecuteSolutionFunc executeChallenge = reinterpret_cast<ExecuteSolutionFunc>(GetProcAddress(m_handle, "ExecuteSolution"));
#else
        ExecuteSolutionFunc executeChallenge = reinterpret_cast<ExecuteSolutionFunc>(dlsym(handle, "ExecuteSolution"));
#endif
        if (!executeChallenge)
            return -1;

        executeChallenge(day, part);

        return 0;
    }
};

int main(int argc, const char *argv[])
{
    InCommand::CCommandReader CommandReader("aocrunner", "Advent of Code Runner");
    InCommand::Int Year(2024);
    InCommand::Int Day(0);
    InCommand::Int Part(0);
    InCommand::Bool ShowHelp;
    InCommand::Bool ListSolutions;
    CommandReader.DeclareSwitchOption(Year, "year", "Advent of Code Year. Default to the latest year.", 'y');
    CommandReader.DeclareSwitchOption(Day, "day", "Advent of Code Day.", 'd');
    CommandReader.DeclareSwitchOption(Part, "part", InCommand::Domain(1, 2), "Indicates which part of a day's challenge to run (possibly ignored).", 'p');
    CommandReader.DeclareBoolSwitchOption(ShowHelp, "help", "Get help", 'h');
    CommandReader.DeclareBoolSwitchOption(ListSolutions, "list", "Lists solutions for the given year.");
    CommandReader.ReadArguments(argc, argv);

    Runner runner;

    if(!runner.Load(Year))
        return -1;

    if (ListSolutions)
    {
        runner.ListSolutions();
        return 0;
    }

    return runner.Execute(Day, Part);
}