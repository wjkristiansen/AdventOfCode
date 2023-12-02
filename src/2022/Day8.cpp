#include "pch.h"
#include "SolutionFactory.h"

struct Tree
{
    Tree() = default;
    Tree(int height) : Height(height) {}
    int VisibleCount = 0;
    int Height = 0;
    int VisibleDistLeft = 0;
    int VisibleDistRight = 0;
    int VisibleDistUp = 0;
    int VisibleDistDown = 0;
    int ScenicScore() const { return VisibleDistUp * VisibleDistDown * VisibleDistLeft * VisibleDistRight; }
};

void CSolution<8>::Execute(int)
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
    for (size_t row = 0; row < grid.size(); ++row)
    {
        // From left to right
        int max = -1;
        for (int column = 0; column < grid[0].size(); ++column)
        {
            auto &Tree = grid[row][column];
            // Determine outside visibility
            if ( Tree.Height > max)
            {
                Tree.VisibleCount++;
                max = Tree.Height;
            }

            // Walk back to the left and to measure visual distance
            for(int walkBack = column; walkBack > 0;)
            {
                --walkBack; 
                Tree.VisibleDistLeft++;

                if(Tree.Height <= grid[row][walkBack].Height)
                {
                    break;
                }
            }
        }

        // From right to left
        max = -1;
        for (int column = int(grid[row].size()); column > 0;)
        {
            --column;
            auto &Tree = grid[row][column];

            if (Tree.Height > max)
            {
                Tree.VisibleCount++;
                max = Tree.Height;
            }

            for(int walkBack = column + 1; walkBack < grid[row].size(); ++walkBack)
            {
                Tree.VisibleDistRight++;
                if(Tree.Height <= grid[row][walkBack].Height)
                {
                    break;
                }
            }
        }
    }

    // Scan columns...
    for(int column = 0; column < grid[0].size(); ++column)
    {
        // From top to bottom
        int max = -1;
        for(int row = 0; row < grid.size(); ++row)
        {
            auto &Tree = grid[row][column];
            if(Tree.Height > max)
            {
                Tree.VisibleCount++;
                max = Tree.Height;
            }

            for(int walkBack = row; walkBack > 0;)
            {
                --walkBack; 
                Tree.VisibleDistUp++;

                if(Tree.Height <= grid[walkBack][column].Height)
                {
                    break;
                }
            }
        }

        // From bottom to top
        max = -1;
        for(int row = int(grid.size()); row > 0;)
        {
            --row;
            auto &Tree = grid[row][column];

            if (Tree.Height > max)
            {
                Tree.VisibleCount++;
                max = Tree.Height;
            }

            for(int walkBack = row + 1; walkBack < grid.size(); ++walkBack)
            {
                Tree.VisibleDistDown++;
                if(Tree.Height <= grid[walkBack][column].Height)
                {
                    break;
                }
            }
        }
    }

    // Now count the number of visible trees
    size_t NumVisible = 0;
    int MaxScenicScore = 0;
    for(auto &row : grid)
    {
        for(auto &Tree : row)
        {
            if(Tree.VisibleCount > 0)
                NumVisible++;

            if (Tree.ScenicScore() > MaxScenicScore)
                MaxScenicScore = Tree.ScenicScore();
        }
    }

    std::cout << "Num visible trees: " << NumVisible << std::endl;
    std::cout << "Max scenic score: " << MaxScenicScore << std::endl;
}
