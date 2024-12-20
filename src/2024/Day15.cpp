#include "pch.h"
#include "SolutionFactory.h"

class CSolution15 : public CSolutionBase
{
public:
    CSolution15() : CSolutionBase()
    {
    }

    AsciiGrid m_Grid;
    std::vector<char> m_MovePattern;
    std::pair<size_t, size_t> m_Bot;

    void ReadMovePattern(std::istream& stream)
    {
        while (!stream.eof())
        {
            auto c = stream.get();
            switch (c)
            {
            case '<':
            case '>':
            case '^':
            case 'v':
                m_MovePattern.push_back(c);
                break;
            default:
                // Ignore
                break;
            }
        }
    }

    std::pair<size_t, size_t> LocateBot()
    {
        for (size_t row = 0; row < m_Grid.Height(); ++row)
        {
            for (size_t col = 0; col < m_Grid.Width(); ++col)
            {
                if (m_Grid.GetValue(row, col) == '@')
                {
                    return std::pair(row, col);
                }
            }
        }

        return std::pair(0, 0); // Not found
    }

    void PrintPattern()
    {
        // Insert a line break each 80 columns
        for (size_t index = 0; index < m_MovePattern.size();)
        {
            std::cout << m_MovePattern[index];
            ++index;
            if ((index % 80) == 0)
            {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }

    void TryMove(size_t patternIndex)
    {
        std::stack<std::pair<size_t, size_t>> moveStack;
        moveStack.push(m_Bot);

        while (!moveStack.empty())
        {
            auto loc = moveStack.top();
            std::pair<size_t, size_t> dest;

            switch (m_MovePattern[patternIndex])
            {
            case '^':
                dest = std::pair(loc.first - 1, loc.second);
                break;
            case 'v':
                dest = std::pair(loc.first + 1, loc.second);
                break;
            case '<':
                dest = std::pair(loc.first, loc.second - 1);
                break;
            case '>':
                dest = std::pair(loc.first, loc.second + 1);
                break;
            }
            
            if (m_Grid.GetValue(dest.first, dest.second) == '.')
            {
                // Swap positions above
                moveStack.pop();
                m_Grid.SetValue(dest.first, dest.second, m_Grid.GetValue(loc.first, loc.second));
                m_Grid.SetValue(loc.first, loc.second, '.');
                if (moveStack.size() == 0)
                    m_Bot = dest;
            }
            else if (m_Grid.GetValue(dest.first, dest.second) == 'O')
            {
                moveStack.push(dest);
            }
            else if (m_Grid.GetValue(dest.first, dest.second) == '#')
            {
                // Clear the move stack
                while (!moveStack.empty())
                    moveStack.pop();
            }
        }
    }

    virtual void Execute(int part) final
    {
        std::ifstream fstream("2024/Day15.txt");
        m_Grid.ReadFromStream(fstream);
        m_Bot = LocateBot();
        m_Grid.Print();
        std::cout << "Bot: [" << m_Bot.first << "][" << m_Bot.second << "]" << std::endl;
        std::cout << std::endl;
        ReadMovePattern(fstream);
        PrintPattern();

        for (size_t i = 0; i < m_MovePattern.size(); ++i)
        {
            TryMove(i);
        }
        m_Grid.Print();

        size_t boxGpsSum = 0;
        for (auto row = 1; row < m_Grid.Height() - 1; ++row)
        {
            for (auto col = 1; col < m_Grid.Width() - 1; ++col)
            {
                if (m_Grid.GetValue(row, col) == 'O')
                    boxGpsSum += 100 * row + col;
            }
        }
        std::cout << "Box GPU Sum: " << boxGpsSum << std::endl;
    }
};

DECLARE_SOLUTION(CSolution15, 15, "Warehouse Woes");
