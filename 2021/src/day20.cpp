#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>

#include "day20.h"

namespace Day20
{
    class Image
    {
        std::deque<std::string> m_Input;
        std::string m_Algorithm;
        bool BoundsState = false;

    public:
        void ReadFile(const std::string& fileName)
        {
            std::ifstream fstream(fileName);
            std::string line;
            std::getline(fstream, line);
            m_Algorithm = line;
            for (; !fstream.eof();)
            {
                std::getline(fstream, line);
                m_Input.push_back(line);
            }
        }

        int DecodeBits(int x, int y)
        {
            int value = 0;
            int mask = 1 << 8;
            for (int j = y - 1; j <= y + 1; ++j)
            {
                for (int i = x - 1; i <= x + 1; ++i)
                {
                    if (i >= 0 && j >= 0 && j < m_Input.size() && i < m_Input[j].length())
                    {
                        if (m_Input[j][i] == '#')
                        {
                            value |= mask;
                        }
                    }
                    else
                    {
                        if (BoundsState)
                            value |= mask;
                    }
                    mask = mask >> 1;
                }
             }

            return value;
        }

        int Enhance()
        {
            int LitCount = 0;
            int width = int(m_Input.front().length());
            int height = int(m_Input.size());
            int outputWidth = width + 2;
            int outputHeight = height + 2;
            std::deque<std::string> Output;

            for (int y = 0; y < outputWidth; ++y)
            {
                Output.push_back("");
                for (int x = 0; x < outputHeight; ++x)
                {
                    int value = DecodeBits(x - 1, y - 1);
                    char c = m_Algorithm[value];
                    Output.back().push_back(c);
                    if (c == '#')
                        LitCount++;
                }
            }

            m_Input = Output;

            BoundsState = m_Algorithm[DecodeBits(-2, -2)] == '#';

            return LitCount;
        }

        void Print()
        {
            for (const std::string &row : m_Input)
            {
                std::cout << row << std::endl;
            }
        }
    };

    void Execute()
    {
        Image img;
        img.ReadFile("day20.txt");
        int LitCount = 0;
        for (int i = 0; i < 50; ++i)
        {
            LitCount = img.Enhance();
        }
        img.Print();
        std::cout << "Lit Count: " << LitCount << std::endl;
    }
}
