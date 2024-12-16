#include "pch.h"
#include "SolutionFactory.h"

class CSolution8: public CSolutionBase
{
public:
    CSolution8() : CSolutionBase()
    {
    }

    using GridType = std::vector<std::vector<char>>;
    using LocationType = std::pair<int, int>;
    using AntennaMapType = std::unordered_map<char, std::vector<LocationType>>;
    GridType m_AntennaGrid;
    GridType m_AntinodeGrid;
    AntennaMapType m_AntennaMap;
    size_t m_UniqueAntinodes = 0;

    void ReadAntennaGrid()
    {
        std::ifstream fstream("2024/Day8.txt");
        std::string line;

        for (;!fstream.eof();)
        {
            std::getline(fstream, line);
            if (line.empty())
                break;
            m_AntennaGrid.push_back({});
            m_AntennaGrid.back().assign(line.begin(), line.end());
            m_AntinodeGrid.emplace_back(m_AntennaGrid.back().size(), '.');
        }
    }

    void BuildAntennaMap()
    {
        for (int i = 0; i < m_AntennaGrid.size(); ++i)
        {
            for (int j = 0; j < m_AntennaGrid.front().size(); ++j)
            {
                auto antenna = m_AntennaGrid[i][j];
                if (antenna != '.')
                {
                    m_AntennaMap[antenna].push_back(LocationType(i, j));
                }
            }
        }
    }

    void PrintGrid(const GridType &grid)
    {
        for (auto row : grid)
        {
            for (auto col : row)
            {
                std::cout << col;
            }

            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void PrintAntennaMap()
    {
        for (const auto &antennaEntry : m_AntennaMap)
        {
            std::cout << antennaEntry.first << std::endl;
            for (const auto& location : antennaEntry.second)
            {
                std::cout << "  " << "[" << location.first << "," << location.second << "]" << std::endl;
            }
        }
    }

    bool SetAntinode(LocationType pos)
    {
        if (pos.first >= 0 && pos.first < m_AntinodeGrid.size() &&
            pos.second >= 0 && pos.second < m_AntennaGrid.front().size())
        {
            if ('#' != m_AntinodeGrid[pos.first][pos.second])
            {
                m_AntinodeGrid[pos.first][pos.second] = '#';
                m_UniqueAntinodes++;
            }
            return true;
        }

        return false;
    }

    virtual void Execute(int part)
    {
        m_AntennaGrid.clear();
        m_AntinodeGrid.clear();
        m_AntennaMap.clear();
        m_UniqueAntinodes = 0;

        if (part == 1)
        {
            ReadAntennaGrid();
            PrintGrid(m_AntennaGrid);
            PrintGrid(m_AntinodeGrid);
            BuildAntennaMap();
            PrintAntennaMap();

            // For each type of antenna
            for (const auto& antennaEntry : m_AntennaMap)
            {
                // Iterate over all unique pairs of locations
                for (int a = 0; a < antennaEntry.second.size(); ++a)
                {
                    for (int b = a + 1; b < antennaEntry.second.size(); ++b)
                    {
                        const LocationType &pos1 = antennaEntry.second[a];
                        const LocationType& pos2 = antennaEntry.second[b];
                        std::pair<int, int> delta(pos2.first - pos1.first, pos2.second - pos1.second);
                        LocationType antiPos1(pos1.first - delta.first, pos1.second - delta.second);
                        LocationType antiPos2(pos2.first + delta.first, pos2.second + delta.second);
                        SetAntinode(antiPos1);
                        SetAntinode(antiPos2);
                    }
                }
            }

            std::cout << "Part 1: Unique Antinode Locations: " << m_UniqueAntinodes << std::endl;
        }
        else
        {
            ReadAntennaGrid();
            BuildAntennaMap();

            // For each type of antenna
            for (const auto& antennaEntry : m_AntennaMap)
            {
                // Iterate over all unique pairs of locations
                for (int a = 0; a < antennaEntry.second.size(); ++a)
                {
                    for (int b = a + 1; b < antennaEntry.second.size(); ++b)
                    {
                        LocationType pos1 = antennaEntry.second[a];
                        LocationType pos2 = antennaEntry.second[b];
                        std::pair<int, int> delta(pos2.first - pos1.first, pos2.second - pos1.second);

                        // Place antinodes at and "behind" pos1
                        for (; SetAntinode(pos1); pos1.first -= delta.first, pos1.second -= delta.second)
                        {
                        }
                        
                        // Place antinodes at and "ahead of" pos2
                        for (; SetAntinode(pos2); pos2.first += delta.first, pos2.second += delta.second)
                        {
                        }
                    }
                }
            }

            std::cout << "Part 2: Unique Antinode Locations: " << m_UniqueAntinodes << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution8, 8, "Resonant Collinearity");
