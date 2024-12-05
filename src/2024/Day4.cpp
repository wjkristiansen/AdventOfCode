#include "pch.h"
#include "SolutionFactory.h"

class CSolution4: public CSolutionBase
{
public:
    CSolution4() : CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        int minPart = part > 0 ? part : 1;
        int maxPart = part > 0 ? part : 2;
        for (int runPart = minPart; runPart <= maxPart; ++runPart)
        {
            if (runPart == 1)
            {
                std::vector<std::vector<char>> wordSearch;
                std::ifstream fstream("2024/Day4.txt");
                for (int i = 0;!fstream.eof();++i)
                {
                    std::string line;
                    std::getline(fstream, line);
                    wordSearch.push_back({});
                    auto& row = wordSearch.back();
                    row.reserve(line.size());
                    std::transform(line.begin(), line.end(), std::back_inserter(row),
                        [](char c) {
                            switch (c)
                            {
                            case 'X':
                            case 'M':
                            case 'A':
                            case 'S':
                                return c;
                                break;
                            default:
                                return '.';
                            }
                        }
                    );
                }

                auto GetLetter = [&wordSearch](int i, int j)
                    {
                        if (i >= wordSearch.size())
                            return '.';
                        if (j >= wordSearch[0].size())
                            return '.';
                        return wordSearch[i][j];
                    };
                // Use a sliding 4x4 window and count the number of 
                // times the letters 'X', 'M', 'A', and 'S' appear in
                // sequence vertically, horizontally, or diagonally
                int xmasCount = 0;
                for (int j = 0; j < wordSearch.size(); ++j)
                {
                    for (int i = 0; i < wordSearch[0].size(); ++i)
                    {
                        // Horizontal
                        if (GetLetter(i, j) == 'X' && GetLetter(i, j + 1) == 'M' && GetLetter(i, j + 2) == 'A' && GetLetter(i, j + 3) == 'S')
                            xmasCount++;
                        else if (GetLetter(i, j) == 'S' && GetLetter(i, j + 1) == 'A' && GetLetter(i, j + 2) == 'M' && GetLetter(i, j + 3) == 'X')
                            xmasCount++;

                        // Vertical
                        if (GetLetter(i, j) == 'X' && GetLetter(i + 1, j) == 'M' && GetLetter(i + 2, j) == 'A' && GetLetter(i + 3, j) == 'S')
                            xmasCount++;
                        else if (GetLetter(i, j) == 'S' && GetLetter(i + 1, j) == 'A' && GetLetter(i + 2, j) == 'M' && GetLetter(i + 3, j) == 'X')
                            xmasCount++;

                        // Diagonal 1
                        if (GetLetter(i, j) == 'X' && GetLetter(i + 1, j + 1) == 'M' && GetLetter(i + 2, j + 2) == 'A' && GetLetter(i + 3, j + 3) == 'S')
                            xmasCount++;
                        else if (GetLetter(i, j) == 'S' && GetLetter(i + 1, j + 1) == 'A' && GetLetter(i + 2, j + 2) == 'M' && GetLetter(i + 3, j + 3) == 'X')
                            xmasCount++;

                        // Diagonal 2
                        if (GetLetter(i + 3, j) == 'X' && GetLetter(i + 2, j + 1) == 'M' && GetLetter(i + 1, j + 2) == 'A' && GetLetter(i, j + 3) == 'S')
                            xmasCount++;
                        else if (GetLetter(i + 3, j) == 'S' && GetLetter(i + 2, j + 1) == 'A' && GetLetter(i + 1, j + 2) == 'M' && GetLetter(i, j + 3) == 'X')
                            xmasCount++;
                    }
                }

                std::cout << "Part 1: XMAS Count = " << xmasCount << std::endl;
            }
            else
            {
                std::vector<std::vector<char>> wordSearch;
                std::ifstream fstream("2024/Day4.txt");
                for (int i = 0;!fstream.eof();++i)
                {
                    std::string line;
                    std::getline(fstream, line);
                    wordSearch.push_back({});
                    auto& row = wordSearch.back();
                    row.reserve(line.size());
                    std::transform(line.begin(), line.end(), std::back_inserter(row),
                        [](char c) {
                            switch (c)
                            {
                            case 'X':
                            case 'M':
                            case 'A':
                            case 'S':
                                return c;
                                break;
                            default:
                                return '.';
                            }
                        }
                    );
                }

                auto GetLetter = [&wordSearch](int i, int j)
                    {
                        if (i >= wordSearch.size())
                            return '.';
                        if (j >= wordSearch[0].size())
                            return '.';
                        return wordSearch[i][j];
                    };
                // Use a sliding 3x3 window and count the number of 
                // times the letters 'M', 'A', and 'S' appear in diagonals
                // sequence vertically, horizontally, or diagonally
                int xmasCount = 0;
                for (int j = 0; j < wordSearch.size() - 2; ++j)
                {
                    for (int i = 0; i < wordSearch[0].size() - 2; ++i)
                    {
                        // 'A' must be in the center location of the 3x3
                        if (GetLetter(i + 1, j + 1) != 'A')
                            continue;
                        // Diagonals
                        if (((GetLetter(i, j) == 'M' && GetLetter(i + 2, j + 2) == 'S') ||
                             (GetLetter(i, j) == 'S' && GetLetter(i + 2, j + 2) == 'M')) &&
                            ((GetLetter(i + 2, j) == 'M' && GetLetter(i, j + 2) == 'S') ||
                              GetLetter(i + 2, j) == 'S' && GetLetter(i, j + 2) == 'M'))
                        {
                            xmasCount++;
                        }
                    }
                }

                std::cout << "Part 2: XMAS Count = " << xmasCount << std::endl;
            }
        }
    }
};

DECLARE_SOLUTION(CSolution4, 4, "Ceres Search");
