#include "pch.h"
#include "SolutionFactory.h"

class CSolution6: public CSolutionBase
{
public:
    enum class Direction
    {
        None,
        North,
        South,
        East,
        West,
    };

    enum class StepResult
    {
        Continue,
        OutOfBounds,
        Infinite,
    };

    using GridType = std::vector<std::vector<char>>;
    using PositionType = std::pair<int, int>;

    GridType m_Grid;
    PositionType m_GuardPos;
    Direction m_GuardDir;
    size_t m_VisitedPositions = 0;
    size_t m_InfiniteResultCount = 0;

    CSolution6(): CSolutionBase()
    {
    }

    char GetMark(const PositionType& pos) const
    {
        return m_Grid[pos.first][pos.second];
    }

    void SetMark(const PositionType& pos, char mark)
    {
        m_Grid[pos.first][pos.second] = mark;
    }

    static constexpr char DirectionMark(Direction dir)
    {
        switch (dir)
        {
        case Direction::North:
            return '^';
            break;
        case Direction::South:
            return 'v';
            break;
        case Direction::East:
            return '>';
            break;
        case Direction::West:
            return '<';
            break;
        }

        return '\0';
    }

    StepResult Step()
    {
        if (GetMark(m_GuardPos) == '.')
            SetMark(m_GuardPos, DirectionMark(m_GuardDir));
        Direction TurnDir = Direction::None;
        PositionType NextPos = m_GuardPos;
        switch (m_GuardDir)
        {
        case Direction::North:
            if (m_GuardPos.first == 0)
                return StepResult::OutOfBounds; // Out of bounds
            TurnDir = Direction::East;
            NextPos.first -= 1;
            break;
        case Direction::South:
            if (m_GuardPos.first == int(m_Grid.size() - 1))
                return StepResult::OutOfBounds; // Out of bounds
            TurnDir = Direction::West;
            NextPos.first += 1;
            break;
        case Direction::East:
            if (m_GuardPos.second == (int(m_Grid[0].size() - 1)))
                return StepResult::OutOfBounds; // Out of bounds
            TurnDir = Direction::South;
            NextPos.second += 1;
            break;
        case Direction::West:
            if (m_GuardPos.second == 0)
                return StepResult::OutOfBounds; // Out of bounds
            TurnDir = Direction::North;
            NextPos.second -= 1;
            break;
        default:
            break;
        }

        if ('#' == GetMark(NextPos))
        {
            m_GuardDir = TurnDir;
            return StepResult::Continue;
        }

        if (DirectionMark(m_GuardDir) == GetMark(NextPos))
        {
            // Deja vu, been here before
            return StepResult::Infinite;
        }

        // Mark newly visited position with direction character
        if ('.' == GetMark(NextPos))
            m_VisitedPositions++;

        m_GuardPos = NextPos;
        return StepResult::Continue;
    }

    static PositionType ReadGrid(GridType &grid)
    {
        PositionType startPos(-1, -1);
        grid.clear();
        std::ifstream fstream("2024/Day6.txt");
        for (int i = 0;!fstream.eof();++i)
        {
            std::string line;
            std::getline(fstream, line);
            grid.push_back({});
            grid.back().assign(line.begin(), line.end());
            if (startPos.first < 0)
            {
                for (int j = 0; j < int(grid.back().size()); ++j)
                {
                    if (grid.back()[j] == '^')
                    {
                        startPos = std::make_pair(int(grid.size() - 1), j);
                    }
                }
            }
        }
        return startPos;
    }

    StepResult RunSteps()
    {
        StepResult result = StepResult::Continue;
        while(StepResult::Continue == result)
        {
            result = Step();
        }

        return result;
    }

    static void PrintGrid(const GridType &grid, const PositionType &pos)
    {
        for (size_t row = 0; row < grid.size(); ++row)
        {
            for (size_t col = 0; col < grid[row].size(); ++col)
            {
                auto& elem = grid[row][col];
                std::cout << elem;
            }
            std::cout << std::endl;
        }

        // Print the guard coordinate
        std::cout << "Guard Pos: (" << pos.first << "," << pos.second << ")" << std::endl;
    }

    virtual void Execute(int part)
    {
        if (part == 1)
        {
            m_Grid.clear();
            m_GuardDir = Direction::North;
            m_VisitedPositions = 1;
            m_GuardPos = ReadGrid(m_Grid);

            // Print the grid for brevity
            PrintGrid(m_Grid, m_GuardPos);

            RunSteps();

            // Print the updated map with marks for brevity
            PrintGrid(m_Grid, m_GuardPos);

            std::cout << "Part 1: Unique positions: " << m_VisitedPositions << std::endl;
        }
        else
        {
            GridType origGrid;
            PositionType origPos = ReadGrid(origGrid);
            size_t limit = origGrid.size() * origGrid.size();

            PrintGrid(origGrid, origPos);

            for (size_t addObstRow = 0; addObstRow < origGrid.size(); ++addObstRow)
            {
                for (size_t addObstCol = 0; addObstCol < origGrid[addObstRow].size(); ++addObstCol)
                {
                    // Only run simulation with new obstacles
                    if (origGrid[addObstRow][addObstCol] != '#')
                    {
                        m_Grid = origGrid;
                        m_Grid[addObstRow][addObstCol] = '#'; // Add new obstacle
                        m_GuardPos = origPos;
                        m_GuardDir = Direction::North;
                        m_VisitedPositions = 1;

                        auto result = RunSteps();

                        if (StepResult::Infinite == result)
                        {
                            m_InfiniteResultCount++;
                        }
                    }
                }
            }

            std::cout << "Part 2: Infinite Result Count: " << m_InfiniteResultCount << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution6, 6, "Guard Gallivant");
