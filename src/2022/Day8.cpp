#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>

#include "..\SolutionFactory.h"


struct Tree
{
    Tree() = default;
    Tree(int height) : Height(height) {}
    int VisibleCount = 0;
    int Height = 0;
};

void CSolution<2022, 8>::Execute()
{
    std::ifstream fstream("Day8.txt");
    std::vector<std::vector<Tree>> grid;

    // Read the grid
    for(; !fstream.eof();)
    {
        std::string line;
        std::getline(fstream, line);
        if(line.empty())
            break;

        grid.emplace_back(line.length());

        for(size_t i = 0; i < line.length(); ++i)
        {
            int height = line[i] - '0';
            grid.back()[i] = Tree(height);
        }
    }

    // Scan rows...
    for(auto &row : grid)
    {
        // From left to right
        int max = -1;
        for(auto it = row.begin(); it != row.end(); ++it)
        {
            if(it->Height > max)
            {
                it->VisibleCount++;
                max = it->Height;
            }
        }

        // From right to left
        max = -1;
        for(auto it = row.rbegin(); it != row.rend(); ++it)
        {
            if(it->Height > max)
            {
                it->VisibleCount++;
                max = it->Height;
            }
        }
    }

    // Scan columns...
    for(size_t column = 0; column < grid[0].size(); ++column)
    {
        // From top to bottom
        int max = -1;
        for(size_t row = 0; row < grid.size(); ++row)
        {
            if(grid[row][column].Height > max)
            {
                grid[row][column].VisibleCount++;
                max = grid[row][column].Height;
            }
        }

        // From bottom to top
        max = -1;
        for(size_t row = grid.size(); row > 0;)
        {
            --row;

            if(grid[row][column].Height > max)
            {
                grid[row][column].VisibleCount++;
                max = grid[row][column].Height;
            }
        }
    }

    // Now count the number of visible trees
    size_t NumVisible = 0;
    for(auto &row : grid)
    {
        for(auto &Tree : row)
        {
            if(Tree.VisibleCount > 0)
                NumVisible++;
        }
    }

    std::cout << "Num visible trees: " << NumVisible << std::endl;
}
