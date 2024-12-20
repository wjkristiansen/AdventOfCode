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
    size_t m_NumSides = 0;

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

            m_NumSides += NumEdgeStarts(row, col);

            // Mark the associated mask location
            m_Mask.SetValue(row, col, 1);
            ++regionSize;

            plotStack.push(std::pair(row - 1, col));
            plotStack.push(std::pair(row + 1, col));
            plotStack.push(std::pair(row, col - 1));
            plotStack.push(std::pair(row, col + 1));
        }

        return std::pair(regionSize, perimeter);
    }

    int NumEdgeStarts(size_t row, size_t col)
    {
        // An edge starts at a corner. There are multiple
        // geometries that represent a corner:
        // Assume edges are counted clockwise, the 
        // ASCII drawing below shows all possible
        // edge orientations.
        //
        //     ^--->
        //     | X |
        // ^--->   v--->
        // | X   X   X |
        // <---^   <---v
        //     | X |
        //     <---v
        //
        // An edge must *start* at a give location, so
        // vertically or horizontally adjacent plots
        // do not count as an edge.
        //
        // ^----------->
        // | X   X   X |
        // <---^   <---v
        //     | X |
        //     <---v
        //

        char c = m_Grid.GetValue(row, col);

        bool leftEdge = (col == 0 || m_Grid.GetValue(row, col - 1) != c);
        bool topEdge = (row == 0 || m_Grid.GetValue(row - 1, col) != c);
        bool rightEdge = (col == m_Grid.Width() - 1 || m_Grid.GetValue(row, col + 1) != c);
        bool bottomEdge = (row == m_Grid.Height() - 1 || m_Grid.GetValue(row + 1, col) != c);

        int numEdgeStarts = 0;

        // Convex edges
        if (leftEdge && topEdge)
            numEdgeStarts++; // Start of a convex top edge
        if (topEdge && rightEdge)
            numEdgeStarts++; // Start of a convex right edge
        if (rightEdge && bottomEdge)
            numEdgeStarts++; // Start of a convex bottom edge
        if (bottomEdge && leftEdge)
            numEdgeStarts++; // Start of a convex left edge

        // Concave edges
        if (!leftEdge && !topEdge && m_Grid.GetValue(row - 1, col - 1) != c)
            numEdgeStarts++; // Start of concave left edge
        if (!topEdge && !rightEdge && m_Grid.GetValue(row - 1, col + 1) != c)
            numEdgeStarts++; // Start of concave top edge
        if (!rightEdge && !bottomEdge && m_Grid.GetValue(row + 1, col + 1) != c)
            numEdgeStarts++; // Start of concave right edge
        if (!bottomEdge && !leftEdge && m_Grid.GetValue(row + 1, col - 1) != c)
            numEdgeStarts++; // Start of concave bottom edge

        return numEdgeStarts;
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
                    size_t initNumSides = m_NumSides;
                    char c = m_Grid.GetValue(row, col);
                    auto [regionSize, perimeter] = ComputeRegion(row, col, c);
                    size_t regionSides = m_NumSides - initNumSides;
                    std::cout << "Region " << c << ": Size=" << regionSize << ", Sides=" << regionSides << std::endl;
                    cost += regionSize * regionSides;
                }
            }
            std::cout << "Total Cost: " << cost << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution12, 12, "Garden Groups");
