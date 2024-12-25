#include "pch.h"
#include <unistd.h>

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
        filename << "./aoc" << year;

#ifdef _WIN32
        filename << ".dll";
        m_handle = LoadLibrary(filename.str().c_str());

        if (!m_handle)
            return false;
#else
        char buffer[512];
        
        // Get the current working directory
        if (getcwd(buffer, sizeof(buffer)) != nullptr)
        {
            std::cout << "Current working directory: " << buffer << std::endl;
        }
        else
        {
            std::cerr << "Error getting current working directory" << std::endl;
        }

        filename << ".so";

        m_handle = dlopen(filename.str().c_str(), RTLD_LAZY);
        if (!m_handle)
        {
            std::cerr << "Failed to load library: " << dlerror() << std::endl;
            return false;
        }
#endif
    
        return true;
    }

    void Unload()
    {
#ifdef _WIN32
        FreeLibrary(m_handle);
#else
        if(m_handle)
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
        ListSolutionsFunc listSolutions = reinterpret_cast<ListSolutionsFunc>(dlsym(m_handle, "ListSolutions"));
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
        ExecuteSolutionFunc executeChallenge = reinterpret_cast<ExecuteSolutionFunc>(dlsym(m_handle, "ExecuteSolution"));
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
    InCommand::String PartString("1");
    InCommand::Bool ShowHelp;
    InCommand::Bool ListSolutions;
    CommandReader.DeclareSwitchOption(Year, "year", "Advent of Code Year. Default to the latest year.", 'y');
    CommandReader.DeclareSwitchOption(Day, "day", "Advent of Code Day.", 'd');
    std::string partDomainValues[] =
    {
        std::string("1"),
        std::string("2"),
        std::string("All"),
    };
    
    CommandReader.DeclareSwitchOption(PartString, "part", InCommand::Domain(3, partDomainValues), "Indicates which part of a day's challenge to run (possibly ignored).", 'p');
    CommandReader.DeclareBoolSwitchOption(ShowHelp, "help", "Get help", 'h');
    CommandReader.DeclareBoolSwitchOption(ListSolutions, "list", "Lists solutions for the given year.");
    CommandReader.ReadArguments(argc, argv);

    Runner runner;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (!runner.Load(Year))
        return -1;

    if (ListSolutions)
    {
        runner.ListSolutions();
        return 0;
    }

    int minPart = 1;
    int maxPart = 2;

    if (PartString.Value() == "1")
        maxPart = 1;
    else if (PartString.Value() == "2")
        minPart = 2;

    for (int Part = minPart; Part <= maxPart; ++Part)
    {
        int result = runner.Execute(Day, Part);
        if (result != 0)
            return result;
    }

    return 0;
}