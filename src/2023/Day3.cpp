#include "pch.h"
#include "SolutionFactory.h"

void CSolution<3>::Execute(int part)
{
    std::ifstream fstream("2023/Day3.txt");
    std::string line;

    std::vector<std::vector<char>> Grid;

    struct NumberEntry
    {
        int value = 0;
        int row = 0;
        int colMin = 0;
        int colMax = 0;
        bool isAdjacentToSymbol = false;
    };

    std::vector<NumberEntry> NumberEntries;

    struct Gear
    {
        int ratio = 1;
        int count = 0;
    };
    std::unordered_map<int, Gear> Gears;

    int row = 0;
    for(;!fstream.eof(); ++row)
    {
        std::getline(fstream, line);
        if(line.empty())
            break;

        // Cache the row data
        Grid.emplace_back(std::vector<char>(line.begin(), line.end()));

        NumberEntry numEntry{};

        for(int col = 0; col < line.length(); ++col)
        {
            if(line[col] >= '0' && line[col] <= '9')
            {
                // accumulate number value
                numEntry.value = numEntry.value * 10 + int(line[col] - '0');

                if(numEntry.colMax == 0)
                {
                    // New number
                    numEntry.colMin = col;
                    numEntry.colMax = col;
                    numEntry.row = row;
                }

                numEntry.colMax++;
            }
            else
            {
                // Flush and reset number entry
                if(numEntry.colMax > 0)
                {
                    NumberEntries.push_back(numEntry);
                    numEntry = NumberEntry{};
                }
            }
        }

        // Flush any final number entry
        if(numEntry.colMin > 0)
        {
            NumberEntries.push_back(numEntry);
        }
    }

    int maxRow = row;
    int maxCol = int(Grid[0].size());

    auto isSymbol = [](char v) -> bool
        {
            if (v == '.')
                return false;
            if (v >= '0' && v <= '9')
                return false;
            return true;
        };

    auto addGearNumber = [&maxCol, &Gears](int gearRow, int gearCol, int value)
        {
            auto gearId = gearRow * maxCol + gearCol;
            auto it = Gears.find(gearId);
            if (it != Gears.end())
            {
                Gear& g = it->second;
                g.ratio *= value;
                g.count++;
            }
            else
            {
                Gears.emplace(gearId, Gear{ value, 1 });
            }
        };

    int adjacentSum = 0;
    int numAdjacentNumbers = 0;
    // Iterate through the number entries and look for adjacent symbols
    for (NumberEntry &numEntry : NumberEntries)
    {
        int col = 0;
        row = numEntry.row - 1;
        bool isAdjacent = false;

        if (row >= 0)
        {
            // Row above...
            for (col = numEntry.colMin - 1; col < numEntry.colMax + 1; ++col)
            {
                if (col >= 0 && col <maxCol)
                {
                    if (isSymbol(Grid[row][col]))
                    {
                        adjacentSum += numEntry.value;
                        numAdjacentNumbers++;
                        numEntry.isAdjacentToSymbol = true;
                        isAdjacent = true; // Signal that this entry has been counted
                        if (Grid[row][col] == '*')
                        {
                            addGearNumber(row, col, numEntry.value);
                        }
                        break;
                    }
                }
            }
        }

        if (!isAdjacent)
        {
            // To the left...
            ++row;
            col = numEntry.colMin - 1;
            if (col >= 0)
            {
                if (isSymbol(Grid[row][col]))
                {
                    adjacentSum += numEntry.value;
                    numAdjacentNumbers++;
                    numEntry.isAdjacentToSymbol = true;
                    isAdjacent = true; // Signal that this entry has been counted
                    if (Grid[row][col] == '*')
                    {
                        addGearNumber(row, col, numEntry.value);
                    }
                }
            }
        }

        if (!isAdjacent)
        {
            // To the right...
            col = numEntry.colMax;
            if (col < maxCol)
            {
                if (isSymbol(Grid[row][col]))
                {
                    adjacentSum += numEntry.value;
                    numAdjacentNumbers++;
                    numEntry.isAdjacentToSymbol = true;
                    isAdjacent = true; // Signal that this entry has been counted
                    if (Grid[row][col] == '*')
                    {
                        addGearNumber(row, col, numEntry.value);
                    }
                }
            }
        }

        // Row below...
        ++row;
        if (row < maxRow)
        {
            for (col = numEntry.colMin - 1; col < numEntry.colMax + 1; ++col)
            {
                if (col >= 0 && col < maxCol)
                {
                    if (isSymbol(Grid[row][col]))
                    {
                        adjacentSum += numEntry.value;
                        numAdjacentNumbers++;
                        numEntry.isAdjacentToSymbol = true;
                        isAdjacent = true; // Signal that this entry has been counted
                        if (Grid[row][col] == '*')
                        {
                            addGearNumber(row, col, numEntry.value);
                        }
                        break;
                    }
                }
            }
        }
    }

    int numRealGears = 0;
    int sumGearRatios = 0;
    for (auto [_, g] : Gears)
    {
        if (g.count == 2)
        {
            numRealGears++;
            sumGearRatios += g.ratio;
        }
    }

    std::cout << "Total number entries: " << NumberEntries.size() << std::endl;
    std::cout << "Total entries with adjacent symbols: " << numAdjacentNumbers << std::endl;
    std::cout << "Sum of numbers adjacent to symbols: " << adjacentSum << std::endl;
    std::cout << "Num actual gears: " << numRealGears << std::endl;
    std::cout << "Sum gear ratios: " << sumGearRatios << std::endl;
}
