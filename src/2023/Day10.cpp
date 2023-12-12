#include "pch.h"
#include "SolutionFactory.h"

class CSolution10 : public CSolutionBase
{
public:
    CSolution10() : CSolutionBase()
    {
    }

    std::vector<std::string> m_PipeGrid;

    enum COMPASS_DIR
    {
        COMPASS_DIR_NORTH = 1 << 0,
        COMPASS_DIR_SOUTH = 1 << 1,
        COMPASS_DIR_EAST = 1 << 2,
        COMPASS_DIR_WEST = 1 << 3,
    };

    COMPASS_DIR Step(COMPASS_DIR in, int& x, int& y)
    {
        COMPASS_DIR direction;

        char &shape = m_PipeGrid[y][x];

        switch (in)
        {
        case COMPASS_DIR_NORTH:
            switch(shape)
            {
            case '|':
                shape = '!';
                y += 1;
                direction = COMPASS_DIR_NORTH;
                break;
            case 'L':
                shape = 'l';
                x += 1;
                direction = COMPASS_DIR_WEST;
                break;
            case 'J':
                shape = 'j';
                x -= 1;
                direction = COMPASS_DIR_EAST;
                break;
            default:
                assert(!"Unexpected pipe shape");
                break;
            }
            break;
        case COMPASS_DIR_SOUTH:
            switch(shape)
            {
            case '|':
                shape = '!';
                y -= 1;
                direction = COMPASS_DIR_SOUTH;
                break;
            case '7':
                shape = '&';
                x -= 1;
                direction = COMPASS_DIR_EAST;
                break;
            case 'F':
                shape = 'f';
                x += 1;
                direction = COMPASS_DIR_WEST;
                break;
            default:
                assert(!"Unexpected pipe shape");
                break;
            }
            break;
        case COMPASS_DIR_EAST:
            switch(shape)
            {
            case '-':
                shape = '_';
                x -= 1;
                direction = COMPASS_DIR_EAST;
                break;
            case 'L':
                shape = 'l';
                y -= 1;
                direction = COMPASS_DIR_SOUTH;
                break;
            case 'F':
                shape = 'f';
                y += 1;
                direction = COMPASS_DIR_NORTH;
                break;
            default:
                assert(!"Unexpected pipe shape");
                break;
            }
            break;
        case COMPASS_DIR_WEST:
            switch(shape)
            {
            case '-':
                shape = '_';
                x += 1;
                direction = COMPASS_DIR_WEST;
                break;
            case '7':
                shape = '&';
                y += 1;
                direction = COMPASS_DIR_NORTH;
                break;
            case 'J':
                shape = 'j';
                y -= 1;
                direction = COMPASS_DIR_SOUTH;
                break;
            default:
                assert(!"Unexpected pipe shape");
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
        COMPASS_DIR fromDirection[2];

        // Identify the two possible directions to move from start
        int pathIndex = 0;
        int dirMask = 0;
        if (coordY > 0)
        {
            char shape = m_PipeGrid[coordY - 1][coordX];
            if (shape == '|' || shape == 'F' || shape == '7')
            {
                seekX[pathIndex] = coordX;
                seekY[pathIndex] = coordY - 1;
                auto dir = COMPASS_DIR_SOUTH;
                fromDirection[pathIndex] = dir;
                dirMask = dirMask | dir;
                pathIndex++;
            }
        }

        if (coordY < gridHeight - 1 && pathIndex < 2)
        {
            char shape = m_PipeGrid[coordY + 1][coordX];
            if (shape == '|' || shape == 'J' || shape == 'L')
            {
                seekX[pathIndex] = coordX;
                seekY[pathIndex] = coordY + 1;
                auto dir = COMPASS_DIR_NORTH;
                fromDirection[pathIndex] = dir;
                dirMask = dirMask | dir;
                pathIndex++;
            }
        }

        if (coordX > 0 && pathIndex < 2)
        {
            char shape = m_PipeGrid[coordY][coordX - 1];
            if (shape == '-' || shape == 'F' || shape == 'L')
            {
                seekX[pathIndex] = coordX - 1;
                seekY[pathIndex] = coordY;
                auto dir = COMPASS_DIR_EAST;
                fromDirection[pathIndex] = dir;
                dirMask = dirMask | dir;
                pathIndex++;
            }
        }

        if (coordX < gridWidth - 1 && pathIndex < 2)
        {
            char shape = m_PipeGrid[coordY][coordX + 1];
            if (shape == '-' || shape == 'J' || shape == '7')
            {
                seekX[pathIndex] = coordX + 1;
                seekY[pathIndex] = coordY;
                auto dir = COMPASS_DIR_WEST;
                fromDirection[pathIndex] = dir;
                dirMask = dirMask | dir;
                pathIndex++;
            }
        }

        // Replace the 'S' with either equivalent pipe symbol
        switch (dirMask)
        {
        case COMPASS_DIR_NORTH | COMPASS_DIR_SOUTH:
            m_PipeGrid[coordY][coordX] = '!';
            break;
        case COMPASS_DIR_EAST | COMPASS_DIR_WEST:
            m_PipeGrid[coordY][coordX] = '_';
            break;
        case COMPASS_DIR_EAST | COMPASS_DIR_NORTH:
            m_PipeGrid[coordY][coordX] = '&';
            break;
        case COMPASS_DIR_EAST | COMPASS_DIR_SOUTH:
            m_PipeGrid[coordY][coordX] = 'j';
            break;
        case COMPASS_DIR_WEST | COMPASS_DIR_NORTH:
            m_PipeGrid[coordY][coordX] = 'f';
            break;
        case COMPASS_DIR_WEST | COMPASS_DIR_SOUTH:
            m_PipeGrid[coordY][coordX] = 'l';
            break;
        default:
            m_PipeGrid[coordY][coordX] = '0'; // Error
            break;
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

        for (auto& row : m_PipeGrid)
        {
            std::cout << row << std::endl;
        }
        std::cout << std::endl;

        // One extra step to patch the last pipe section with either '!' or '_'
        fromDirection[0] = Step(fromDirection[0], seekX[0], seekY[0]);

        // Scan for interior spaces in grid
        // Each encounter with '!' toggles the in/out status
        int insideCount = 0;
        for (coordY = 0; coordY < gridHeight; ++coordY)
        {
            bool isInside = false;
            int edgeMask = 0;
            for (coordX = 0; coordX < gridWidth; ++coordX)
            {
                switch (m_PipeGrid[coordY][coordX])
                {
                case 'l':
                    edgeMask = COMPASS_DIR_SOUTH;
                    isInside = !isInside;
                    break;
                case 'f':
                    edgeMask = COMPASS_DIR_NORTH;
                    isInside = !isInside;
                    break;
                case 'j':
                    if(edgeMask == COMPASS_DIR_SOUTH)
                        isInside = !isInside; // U-turn
                    edgeMask = 0;
                    break;
                case '&':
                    if (edgeMask == COMPASS_DIR_NORTH)
                        isInside = !isInside; // U-turn
                    edgeMask = 0;
                    break;
                case '!':
                    isInside = !isInside;
                    break;
                case '_':
                    break;
                default:
                    if (isInside)
                    {
                        ++insideCount;
                        m_PipeGrid[coordY][coordX] = 'I';
                    }
                    else
                    {
                        m_PipeGrid[coordY][coordX] = 'O';
                    }
                    break;
                }
            }
        }

        for (auto &row : m_PipeGrid)
        {
            std::cout << row << std::endl;
        }

        std::cout << "Steps: " << step << std::endl;
        std::cout << "Interior tiles: " << insideCount << std::endl;
    }
};

DECLARE_SOLUTION(CSolution10, 10, "Pipe Maze");
