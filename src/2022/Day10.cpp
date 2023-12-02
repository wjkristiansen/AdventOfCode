#include "pch.h"
#include "SolutionFactory.h"

enum class Op
{
    noop,
    addx,
};

class CPU
{
public:
    int m_x = 1;
    Op m_curOp = Op::noop;
    int m_operand = 0;
    int m_opTicksRemaining = 0;
    std::istream &m_input;
    std::vector<char> &m_screenPixels;
    int m_screenWidth;

    CPU(std::istream &input, std::vector<char> &screenPixels, int screenWidth) :
        m_input(input),
        m_screenPixels(screenPixels),
        m_screenWidth(screenWidth) {}

    bool Cycle(int tick)
    {
        // Executo op
        if(m_opTicksRemaining == 0)
        {
            // Finish addx
            if(m_curOp == Op::addx)
            {
                m_x += m_operand;
            }

            // Fetch command
            if(m_input.eof())
                return false;

            std::string opString;
            m_input >> opString;

            if(opString.empty())
                return false;

            if(opString == "noop")
            {
                m_curOp = Op::noop;
                m_opTicksRemaining = 1;
            }
            else
            {
                assert(opString == "addx");
                m_curOp = Op::addx;
                m_input >> m_operand;
                m_opTicksRemaining = 2;
            }
        }

        m_opTicksRemaining--;

        // Draw pixel
        if (tick < m_screenPixels.size())
        {
            int col = tick % m_screenWidth;
            m_screenPixels[tick] = ((col >= m_x - 1) && (col <= m_x + 1)) ? '#' : '.';
        }

        return true;
    }
};

void CSolution<10>::Execute(int)
{
    std::ifstream fstream("Day10.txt");
    const int screenRows = 6;
    const int screenColumns = 40;
    std::vector<char> screen(screenRows * screenColumns);

    CPU cpu(fstream, screen, screenColumns);

    int signalStrength = 0;

    for(int tick = 0; cpu.Cycle(tick);)
    {
        ++tick;
        switch (tick)
        {
        case 20:
            signalStrength += 20 * cpu.m_x;
            break;
        case 60:
            signalStrength += 60 * cpu.m_x;
            break;
        case 100:
            signalStrength += 100 * cpu.m_x;
            break;
        case 140:
            signalStrength += 140 * cpu.m_x;
            break;
        case 180:
            signalStrength += 180 * cpu.m_x;
            break;
        case 220:
            signalStrength += 220 * cpu.m_x;
            break;
        }
    }

    std::cout << "Signal strength: " << signalStrength << std::endl;

    size_t pixel = 0;
    for(int row = 0; row < screenRows; ++row)
    {
        for(int column = 0; column < screenColumns; ++column)
        {
            std::cout << screen[pixel];
            ++pixel;
        }

        std::cout << std::endl;
    }
}
