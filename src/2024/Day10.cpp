#include "pch.h"
#include "SolutionFactory.h"

class CSolution10: public CSolutionBase
{
public:
    CSolution10() : CSolutionBase()
    {
    }

    DecimalGrid<int> m_Grid;
    DecimalGrid<int> m_MaskGrid;

    int RecursivePathSearch(size_t row, size_t col, int value)
    {
        if (m_MaskGrid.GetValue(row, col) != -1)
            return 0; // Already fully explored

        if (value != m_Grid.GetValue(row, col))
            return 0; // Not on this trail
        
        int result = 0;
        if (value == 9)
        {
            // Mark as found
            result = 1;
        }
        else
        {
            ++value;

            if (row > 0)
            {
                result += RecursivePathSearch(row - 1, col, value);
            }

            if (col > 0)
            {
                result += RecursivePathSearch(row, col - 1, value);
            }

            if (row < m_Grid.Height() - 1)
            {
                result += RecursivePathSearch(row + 1, col, value);
            }

            if (col < m_Grid.Width() - 1)
            {
                result += RecursivePathSearch(row, col + 1, value);
            }
        }

        // Mark location as searched
        m_MaskGrid.SetValue(value, row, col);

        return result;
    }

    int UnmaskedRecursivePathSearch(size_t row, size_t col, int value)
    {
        if (value != m_Grid.GetValue(row, col))
            return 0; // Not on this trail
        
        int result = 0;
        if (value == 9)
        {
            // Mark as found
            result = 1;
        }
        else
        {
            ++value;

            if (row > 0)
            {
                result += UnmaskedRecursivePathSearch(row - 1, col, value);
            }

            if (col > 0)
            {
                result += UnmaskedRecursivePathSearch(row, col - 1, value);
            }

            if (row < m_Grid.Height() - 1)
            {
                result += UnmaskedRecursivePathSearch(row + 1, col, value);
            }

            if (col < m_Grid.Width() - 1)
            {
                result += UnmaskedRecursivePathSearch(row, col + 1, value);
            }
        }

        return result;
    }

    virtual void Execute(int part)
    {
        if (part == 1)
        {
            m_Grid.ReadDenseSingleDigitsFromFile("2024/Day10.txt");
            m_Grid.Print();
            std::cout << std::endl;
            int trailSum = 0;

            // For each trailhead (grid value 0)...
            for (size_t row = 0; row < m_Grid.Height(); ++row)
            {
                for (size_t col = 0; col < m_Grid.Width(); ++col)
                {
                    // Reset the mask grid
                    m_MaskGrid = DecimalGrid<int>(m_Grid.Height(), m_Grid.Width(), -1);

                    if (m_Grid.Data()[row][col] == 0)
                    {
                        int score = RecursivePathSearch(row, col, 0);
                        trailSum += score;
                        std::cout << "Trailhead [" << row << "][" << col << "]: " << score << std::endl;
                    }
                }
            }

            std::cout << "Part 1: Trail Sum: " << trailSum << std::endl;
        }
        else
        {
            m_Grid.ReadDenseSingleDigitsFromFile("2024/Day10.txt");
            m_Grid.Print();
            std::cout << std::endl;
            int trailSum = 0;

            // For each trailhead (grid value 0)...
            for (size_t row = 0; row < m_Grid.Height(); ++row)
            {
                for (size_t col = 0; col < m_Grid.Width(); ++col)
                {
                    if (m_Grid.Data()[row][col] == 0)
                    {
                        int score = UnmaskedRecursivePathSearch(row, col, 0);
                        trailSum += score;
                        std::cout << "Trailhead [" << row << "][" << col << "]: " << score << std::endl;
                    }
                }
            }

            std::cout << "Part 2: Trail Sum: " << trailSum << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution10, 10, "Hoof It");
