#include "pch.h"
#include "SolutionFactory.h"

class CSolution12: public CSolutionBase
{
public:
    CSolution12() : CSolutionBase()
    {
    }

    AsciiGrid m_Grid;
    Grid<int> m_Mask;

    std::pair<size_t, size_t> ComputeRegion(size_t startRow, size_t startCol, char c)
    {
        size_t perimeter = 0;
        size_t regionSize = 0;
        std::stack<std::pair<size_t, size_t>> plotStack;

        plotStack.push(std::pair(startRow, startCol));
        while (!plotStack.empty())
        {
            auto [row, col] = plotStack.top();
            plotStack.pop();

            if (row < 0 ||
                col < 0 ||
                row >= m_Grid.Height() ||
                col >= m_Grid.Width() ||
                m_Grid.GetValue(row, col) != c)
            {
                if (regionSize)
                    perimeter++;
                continue;
            }

            if (0 != m_Mask.GetValue(row, col))
                continue; // Been here already

            // Mark the associated mask location
            m_Mask.SetValue(1, row, col);
            ++regionSize;

            plotStack.push(std::pair(row - 1, col));
            plotStack.push(std::pair(row + 1, col));
            plotStack.push(std::pair(row, col - 1));
            plotStack.push(std::pair(row, col + 1));
        }

        return std::pair(regionSize, perimeter);
    }

    virtual void Execute(int part)
    {
        if (part == 1)
        {
            m_Grid.ReadFromFile("2024/Day12.txt");
            m_Mask = Grid<int>(m_Grid.Width(), m_Grid.Height(), 0);
            m_Grid.Print();
            std::cout << std::endl;
            m_Mask.Print();

            size_t cost = 0;
            for (size_t row = 0; row < m_Grid.Height(); ++row)
            {
                for (size_t col = 0; col < m_Grid.Width(); ++col)
                {
                    if (m_Mask.GetValue(row, col) != 0)
                        continue;
                    char c = m_Grid.GetValue(row, col);
                    auto [regionSize, perimeter] = ComputeRegion(row, col, c);
                    std::cout << "Region " << c << ": Size=" << regionSize << ", Perimeter=" << perimeter << std::endl;
                    cost += regionSize * perimeter;
                }
            }
            std::cout << "Total Cost: " << cost << std::endl;
        }
        else
        {
        }
    }
};

DECLARE_SOLUTION(CSolution12, 12, "Garden Groups");
