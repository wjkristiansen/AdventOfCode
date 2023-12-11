#include "pch.h"
#include "SolutionFactory.h"

class CSolution11: public CSolutionBase
{
    std::vector<int64_t> m_GalacticRow;
    std::vector<int64_t> m_GalacticCol;
    std::vector<std::string> m_GalaxyMap;
    std::vector<std::pair<int64_t, int64_t>> m_GalaxyCoords;
    const int expansionAmount = 1000000;

    std::pair<int64_t, int64_t> GalacticCoordinate(const std::pair<int64_t, int64_t>& mapCoord)
    {
        return std::make_pair(
            m_GalacticRow[mapCoord.first],
            m_GalacticCol[mapCoord.second]);
    }

    int64_t GalacticDistance(const std::pair<int64_t, int64_t>& coord1, const std::pair<int64_t, int64_t>& coord2)
    {
        return std::abs(coord2.first - coord1.first) + std::abs(coord2.second - coord1.second);
    }

public:
    CSolution11(): CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        std::ifstream fstream("2023/Day11.txt");
        for (;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);

            if (line.empty())
                continue;
            
            m_GalaxyMap.emplace_back(std::move(line));
        }

        const int mapWidth = (int) m_GalaxyMap[0].length();
        const int mapHeight = (int) m_GalaxyMap.size();

        m_GalacticRow.resize(mapWidth);
        m_GalacticCol.resize(mapHeight);

        // Scan for empty rows and columns
        int expandedRow = 0;
        for (int row = 0; row < mapHeight; ++row)
        {
            bool isRowEmpty = true;
            for (int col = 0; col < mapWidth; ++col)
            {
                if (m_GalaxyMap[row][col] == '#')
                {
                    // Temporarily set a 1 as the row width
                    m_GalacticCol[col] = 1;
                    isRowEmpty = false;
                    m_GalaxyCoords.emplace_back(row, col);
                }
            }

            m_GalacticRow[row] = expandedRow;

            expandedRow += isRowEmpty ? expansionAmount : 1;
        }

        // Fixup column widths
        int64_t expandedCol = 0;
        for (int col = 0; col < mapWidth; ++col)
        {
            bool isExpanded = m_GalacticCol[col] == 0;
            m_GalacticCol[col] = expandedCol;

            expandedCol += isExpanded ? expansionAmount : 1;
        }

        // Fixup galaxy coordinates
        for (auto &coord : m_GalaxyCoords)
        {
            coord = GalacticCoordinate(coord);
            std::cout << coord.first << ", " << coord.second << std::endl;
        }

        int64_t sumDistance = 0;
        for (size_t i = 0; i < m_GalaxyCoords.size(); ++i)
        {
            for (size_t j = i; j < m_GalaxyCoords.size(); ++j)
            {
                sumDistance += GalacticDistance(m_GalaxyCoords[i], m_GalaxyCoords[j]);
            }
        }

        std::cout << "Sum distances: " << sumDistance << std::endl;
    }
};

static CSolutionFactory::CSolutionAllocator<CSolution11> SolutionAllocator(11, "Cosmic Expansion");