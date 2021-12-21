#include <iostream>
#include <unordered_set>

#include "input.h"

uint32_t day3_reduce(bool most_common)
{
    std::unordered_set<uint32_t> data;

    for (auto value : g_day3_input)
    {
        data.emplace(value);
    }

    for (int i = 12; i != 0; --i)
    {
        int ones_count = 0;
        int bit_index = i - 1;
        uint32_t mask = (1 << bit_index);

        for (auto value : data)
        {
            int bitvalue = value & mask;
            ones_count += (int)(bitvalue != 0);
        }

        if ((most_common && (ones_count * 2 ) >= data.size()) || (!most_common && (ones_count * 2) < data.size()))
        {
            // Keep 1's
            for (auto it = data.begin(); it != data.end();)
            {
                auto value = *it;
                if (!(value & mask))
                {
                    it = data.erase(it);
                    if (data.size() == 1)
                        return *data.begin();
                }
                else
                {
                    ++it;
                }
            }
        }
        else
        {
            // Keep 0's
            for (auto it = data.begin(); it != data.end();)
            {
                auto value = *it;
                if (value & mask)
                {
                    it = data.erase(it);
                    if (data.size() == 1)
                        return *data.begin();
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    return 0;
}

void day3()
{
    uint32_t o2 = day3_reduce(true);
    uint32_t co2 = day3_reduce(false);
    uint64_t product = o2 * co2;
    std::cout << product;
}

void day2()
{
    const int size = sizeof(g_input2) / sizeof(g_input2[0]);
    int horizontalPos = 0;
    int verticalPos = 0;
    int aim = 0;
    for(int i = 0; i < size; ++i)
    {
        if(g_input2[i].direction == DIRECTION::up)
        {
            aim -= g_input2[i].amount;
        }
        else if(g_input2[i].direction == DIRECTION::down)
        {
            aim += g_input2[i].amount;
        }
        else if(g_input2[i].direction == DIRECTION::forward)
        {
            horizontalPos += g_input2[i].amount;
            verticalPos += aim * g_input2[i].amount;
        }
    }
    std::cout << verticalPos * horizontalPos << std::endl;
}

void day1()
{
    const int size = sizeof(g_input) / sizeof(g_input[0]);
    int numIncreasing = 0;
    for(int i = 3; i < size; ++i)
    {
        if(g_input[i] > g_input[i - 3])
        {
            ++numIncreasing;
        }
    }
    std::cout << numIncreasing << std::endl;
}

int main(int argc, char *argv[])
{
    day3();

    return 0;
}
