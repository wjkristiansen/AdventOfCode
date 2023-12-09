#include "pch.h"
#include "SolutionFactory.h"

void CSolution<9>::Execute(int part)
{
    std::ifstream fstream("2023/Day9.txt");
    std::string line;

    int64_t sum = 0;
    for (; !fstream.eof();)
    {
        std::getline(fstream, line);
        if(line.empty())
            continue; // Skip empty lines

        // Read the top-level series
        std::istringstream ss(line);
        std::deque<int64_t> series;
        bool isAllZeros = true;
        for (;!ss.eof();)
        {
            int64_t value;
            ss >> value;
            series.push_back(value);
            if(value != 0)
                isAllZeros = false;
        }

        std::deque<std::deque<int64_t>> seriesStack;
        // Compose the next series
        for (;;)
        {
            seriesStack.emplace_back(std::move(series));
            series.resize(seriesStack.back().size() - 1);
            bool isAllZeros = true;
            for (int i = 0; i < seriesStack.back().size() - 1; ++i)
            {
                int64_t v1 = seriesStack.back()[i];
                int64_t v2 = seriesStack.back()[i + 1];
                int64_t delta = v2 - v1;
                if (delta != 0)
                    isAllZeros = false;
                series[i] = delta;
            }

            if (isAllZeros)
                break;
        }

        // Calculate the extrapolated value of each series
        for (size_t i = seriesStack.size() - 1; i > 0; --i)
        {
            int64_t extrap = seriesStack[i - 1].front() - seriesStack[i].front(); 
            seriesStack[i - 1].push_front(extrap);
        }

        sum += seriesStack[0].front();
    }

    std::cout << "Top-level extrapolated values: " << sum << std::endl;
}