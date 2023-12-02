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

    int Execute(int day, int part) const
    {
        if (!m_handle)
            return -1;

        typedef void (*ExecuteChallengeFunc)(int day, int part);
#ifdef _WIN32
        ExecuteChallengeFunc executeChallenge = reinterpret_cast<ExecuteChallengeFunc>(GetProcAddress(m_handle, "ExecuteChallenge"));
#else
        ExecuteChallengeFunc executeChallenge = reinterpret_cast<ExecuteChallengeFunc>(dlsym(handle, "ExecuteChallenge"));
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
    InCommand::Int Year(2023);
    InCommand::Int Day(0);
    InCommand::Int Part(0);
    InCommand::Bool ShowHelp;
    CommandReader.DeclareSwitchOption(Year, "year", "Advent of Code Year", 'y');
    CommandReader.DeclareSwitchOption(Day, "day", "Advent of Code Day", 'd');
    CommandReader.DeclareSwitchOption(Part, "part", InCommand::Domain(1, 2), "Indicates which part of a day's challenge to run", 'p');
    CommandReader.DeclareBoolSwitchOption(ShowHelp, "help", "Get help", 'h');
    CommandReader.ReadArguments(argc, argv);

    Runner runner;

    if(!runner.Load(Year))
        return -1;

    return runner.Execute(Day, Part);
}