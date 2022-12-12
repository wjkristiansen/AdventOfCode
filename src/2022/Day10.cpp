#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

#include "..\SolutionFactory.h"

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

    CPU(std::istream &input) :
        m_input(input) {}

    bool Cycle()
    {
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

        return true;
    }
};

void CSolution<2022, 10>::Execute()
{
    std::ifstream fstream("Day10.txt");

    CPU cpu(fstream);

    int signalStrength = 0;

    for(size_t ticks = 0; cpu.Cycle();)
    {
        ++ticks;
        switch (ticks)
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
}
