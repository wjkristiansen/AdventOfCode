#include "pch.h"
#include "SolutionFactory.h"

class CSolution10 : public CSolutionBase
{
public:
    CSolution10() : CSolutionBase(10, "Pipe Maze")
    {
    }

    std::vector<std::string> m_PipeGrid;

    enum class CompassDirection
    {
        North = 0,
        South,
        East,
        West
    };

    CompassDirection Step(CompassDirection in, int& x, int& y)
    {
        CompassDirection direction;

        char tile = m_PipeGrid[y][x];
        switch (in)
        {
        case CompassDirection::North:
            switch(tile)
            {
            case '|':
                y += 1;
                direction = CompassDirection::North;
                break;
            case 'L':
                x += 1;
                direction = CompassDirection::West;
                break;
            case 'J':
                x -= 1;
                direction = CompassDirection::East;
                break;
            default:
                assert(!"Unexpected pipe tile");
                break;
            }
            break;
        case CompassDirection::South:
            switch(tile)
            {
            case '|':
                y -= 1;
                direction = CompassDirection::South;
                break;
            case '7':
                x -= 1;
                direction = CompassDirection::East;
                break;
            case 'F':
                x += 1;
                direction = CompassDirection::West;
                break;
            default:
                assert(!"Unexpected pipe tile");
                break;
            }
            break;
        case CompassDirection::East:
            switch(tile)
            {
            case '-':
                x -= 1;
                direction = CompassDirection::East;
                break;
            case 'L':
                y -= 1;
                direction = CompassDirection::South;
                break;
            case 'F':
                y += 1;
                direction = CompassDirection::North;
                break;
            default:
                assert(!"Unexpected pipe tile");
                break;
            }
            break;
        case CompassDirection::West:
            switch(tile)
            {
            case '-':
                x += 1;
                direction = CompassDirection::West;
                break;
            case '7':
                y += 1;
                direction = CompassDirection::North;
                break;
            case 'J':
                y -= 1;
                direction = CompassDirection::South;
                break;
            default:
                assert(!"Unexpected pipe tile");
                break;
            }
            break;
        }

        return direction;
    }

    virtual void Execute(int part)
    {
        int coordX = 0;
        int coordY = 0;
        bool startPosFound = false;
        // Read the pipe grid
        std::ifstream fstream("2023/Day10.txt");
        for (;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);
            if(!startPosFound)
            {
                // Attempt to locate start coordinate
                auto offset = line.find_first_of('S');
                if (offset != std::string::npos)
                {
                    startPosFound = true;
                    coordX = (int) offset;
                }
                else
                {
                    coordY++;
                }
            }
            m_PipeGrid.emplace_back(std::move(line));
        }

        int gridWidth = (int) m_PipeGrid[0].length();
        int gridHeight = (int)m_PipeGrid.size();
        int seekX[2];
        int seekY[2];
        CompassDirection fromDirection[2];

        // Identify the two possible directions to move from start
        int pathIndex = 0;
        char tile = m_PipeGrid[coordY - 1][coordX];
        if (coordY > 0)
        {
            if (tile == '|' || tile == 'F' || tile == '7')
            {
                seekX[pathIndex] = coordX;
                seekY[pathIndex] = coordY - 1;
                fromDirection[pathIndex] = CompassDirection::South;
                pathIndex++;
            }
        }

        if (coordY < gridHeight - 1 && pathIndex < 2)
        {
            tile = m_PipeGrid[coordY + 1][coordX];
            if (tile == '|' || tile == 'J' || tile == 'L')
            {
                seekX[pathIndex] = coordX;
                seekY[pathIndex] = coordY + 1;
                fromDirection[pathIndex] = CompassDirection::North;
                pathIndex++;
            }
        }

        if (coordX > 0 && pathIndex < 2)
        {
            tile = m_PipeGrid[coordY][coordX - 1];
            if (tile == '-' || tile == 'F' || tile == 'L')
            {
                seekX[pathIndex] = coordX - 1;
                seekY[pathIndex] = coordY;
                fromDirection[pathIndex] = CompassDirection::East;
                pathIndex++;
            }
        }

        if (coordX < gridWidth - 1 && pathIndex < 2)
        {
            tile = m_PipeGrid[coordY][coordX + 1];
            if (tile == '-' || tile == 'J' || tile == '7')
            {
                seekX[pathIndex] = coordX + 1;
                seekY[pathIndex] = coordY;
                fromDirection[pathIndex] = CompassDirection::West;
                pathIndex++;
            }
        }

        // Ensure we found two directions
        assert(pathIndex == 2);

        // Seek both directions from start to find the most distant location
        int step = 1; // Already stepped into adjacent pipes before iterating
        for (;;)
        {
            fromDirection[0] = Step(fromDirection[0], seekX[0], seekY[0]);
            fromDirection[1] = Step(fromDirection[1], seekX[1], seekY[1]);

            ++step;

            if (seekX[0] == seekX[1] && seekY[0] == seekY[1])
                break;
        }

        std::cout << "Steps: " << step << std::endl;
    }
};

static CSolution10 Solution;
