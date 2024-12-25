#include "pch.h"
#include "SolutionFactory.h"

class CSolution15 : public CSolutionBase
{
public:
    CSolution15() : CSolutionBase()
    {
    }

    struct Location
    {
        size_t Row;
        size_t Col;

        Location() = default;
        Location(const Location& o) = default;
        Location(size_t row, size_t col) :
            Row(row),
            Col(col)
        {
        }
    };

    // Only need to use the column in the hasher
    // since related collections are from the same
    // row.
    struct MoveLocationsHash
    {
        size_t operator()(const Location& loc) const
        {
            return loc.Col;
        }
    };

    // Only need to compare column values
    // since related collections are from the same
    // row.
    struct MoveLocationsEqual
    {
        bool operator()(const Location& loc1, const Location& loc2) const
        {
            return loc1.Col == loc2.Col;
        }
    };

    AsciiGrid m_Grid;
    std::vector<char> m_MovePattern;
    Location m_Bot;

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

    Location LocateBot()
    {
        for (size_t row = 0; row < m_Grid.Height(); ++row)
        {
            for (size_t col = 0; col < m_Grid.Width(); ++col)
            {
                if (m_Grid.GetValue(row, col) == '@')
                {
                    return Location(row, col);
                }
            }
        }

        return Location(0, 0); // Not found
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
        std::stack<Location> moveStack;
        moveStack.push(m_Bot);

        while (!moveStack.empty())
        {
            auto loc = moveStack.top();
            Location dest;

            switch (m_MovePattern[patternIndex])
            {
            case '^':
                dest = Location(loc.Row - 1, loc.Col);
                break;
            case 'v':
                dest = Location(loc.Row + 1, loc.Col);
                break;
            case '<':
                dest = Location(loc.Row, loc.Col - 1);
                break;
            case '>':
                dest = Location(loc.Row, loc.Col + 1);
                break;
            }
            
            if (m_Grid.GetValue(dest.Row, dest.Col) == '.')
            {
                // Swap positions above
                moveStack.pop();
                m_Grid.SetValue(dest.Row, dest.Col, m_Grid.GetValue(loc.Row, loc.Col));
                m_Grid.SetValue(loc.Row, loc.Col, '.');
                if (moveStack.size() == 0)
                    m_Bot = dest;
            }
            else if (m_Grid.GetValue(dest.Row, dest.Col) == 'O')
            {
                moveStack.push(dest);
            }
            else if (m_Grid.GetValue(dest.Row, dest.Col) == '#')
            {
                // Clear the move stack
                while (!moveStack.empty())
                    moveStack.pop();
            }
        }
    }

    void TryWideMove(size_t patternIndex)
    {
        using MoveLocationsType = std::unordered_set<Location, MoveLocationsHash, MoveLocationsEqual>;
        std::stack<MoveLocationsType> moveStack;
        auto moveMarker = m_MovePattern[patternIndex];

        moveStack.push(MoveLocationsType{ m_Bot });

        while (!moveStack.empty())
        {
            auto sourceLocations = moveStack.top();
            MoveLocationsType destLocations;

            bool canMove = true;
            bool blocked = false;

            for (auto source : sourceLocations)
            {
                Location destCoord;

                switch (moveMarker)
                {
                case '^':
                    destCoord = { source.Row - 1, source.Col}; // row above
                    break;
                case 'v':
                    destCoord = { source.Row + 1, source.Col }; // row below
                    break;
                case '<':
                    destCoord = { source.Row, source.Col - 1 }; // column to the left
                    break;
                case '>':
                    destCoord = { source.Row, source.Col + 1 }; // column to the right
                    break;
                }

                auto destMarker = m_Grid.GetValue(destCoord.Row, destCoord.Col);
                if (destMarker == '.')
                {
                    continue;
                }

                canMove = false;

                destLocations.emplace(destCoord);

                if (destMarker == '#')
                {
                    // Clear the move stack
                    while (!moveStack.empty())
                        moveStack.pop();
                    blocked = true;
                    break;
                }

                if (destMarker == '[' && moveMarker == '>')
                {
                    // Move the other half of the crate
                    assert(destLocations.size() == 1);
                    moveStack.push(destLocations);
                    destLocations = MoveLocationsType{ Location(destCoord.Row, destCoord.Col + 1) };
                }
                else if (destMarker == ']' && moveMarker == '<')
                {
                    // Move the other half of the crate
                    assert(destLocations.size() == 1);
                    moveStack.push(destLocations);
                    destLocations = MoveLocationsType{ Location(destCoord.Row, destCoord.Col - 1)};
                }
                else if (moveMarker == '^' || moveMarker == 'v')
                {
                    if (destMarker == '[')
                    {
                        // Widen to include other half of crate
                        destLocations.emplace(destCoord.Row, destCoord.Col + 1);
                    }
                    else if (destMarker == ']')
                    {
                        // Widen to include other half of crate
                        destLocations.emplace(destCoord.Row, destCoord.Col - 1);
                    }
                }
            }

            if (canMove)
            {
                moveStack.pop();
                for (auto source : sourceLocations)
                {
                    Location dest;
                    switch (moveMarker)
                    {
                    case '^':
                        dest = Location(source.Row - 1, source.Col);
                        break;
                    case 'v':
                        dest = Location(source.Row + 1, source.Col);
                        break;
                    case '>':
                        dest = Location(source.Row, source.Col + 1);
                        break;
                    case '<':
                        dest = Location(source.Row, source.Col - 1);
                        break;
                    }
                    
                    m_Grid.SetValue(dest.Row, dest.Col, m_Grid.GetValue(source.Row, source.Col));
                    m_Grid.SetValue(source.Row, source.Col, '.');
                    if (moveStack.size() == 0)
                        m_Bot = dest;
                }
            }
            else if (!blocked)
            {
                moveStack.push(destLocations);
            }
        }
    }

    virtual void Execute(int part) final
    {
        std::ifstream fstream("2024/Day15.txt");

        if (part == 1)
        {
            m_Grid.ReadFromStream(fstream);
            m_Bot = LocateBot();
            m_Grid.Print();
            std::cout << "Bot: [" << m_Bot.Row << "][" << m_Bot.Col << "]" << std::endl;
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
        else
        {
            AsciiGrid tempGrid;
            tempGrid.ReadFromStream(fstream);
            m_Grid = AsciiGrid(tempGrid.Width() * 2, tempGrid.Height(), '.');
            for (size_t row = 0; row < tempGrid.Height(); ++row)
            {
                for (size_t col = 0; col < tempGrid.Width(); ++col)
                {
                    switch (tempGrid.GetValue(row, col))
                    {
                    case '@':
                        m_Grid.SetValue(row, 0 + col * 2, '@');
                        m_Grid.SetValue(row, 1 + col * 2, '.');
                        break;
                    case '#':
                        m_Grid.SetValue(row, 0 + col * 2, '#');
                        m_Grid.SetValue(row, 1 + col * 2, '#');
                        break;
                    case 'O':
                        m_Grid.SetValue(row, 0 + col * 2, '[');
                        m_Grid.SetValue(row, 1 + col * 2, ']');
                    }
                }
            }
            m_Bot = LocateBot();
            m_Grid.Print();
            std::cout << "Bot: [" << m_Bot.Row << "][" << m_Bot.Col << "]" << std::endl;
            std::cout << std::endl;
            ReadMovePattern(fstream);

            for (size_t i = 0; i < m_MovePattern.size(); ++i)
            {
                // std::cout << "Move " << m_MovePattern[i] << ":" << std::endl;
                TryWideMove(i);
                // m_Grid.Print();
            }

            size_t boxGpsSum = 0;
            for (auto row = 1; row < m_Grid.Height() - 1; ++row)
            {
                for (auto col = 1; col < m_Grid.Width() - 1; ++col)
                {
                    if (m_Grid.GetValue(row, col) == '[')
                        boxGpsSum += 100 * row + col;
                }
            }
            std::cout << "Box GPU Sum: " << boxGpsSum << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution15, 15, "Warehouse Woes");
