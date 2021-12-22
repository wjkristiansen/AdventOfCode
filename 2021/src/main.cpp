#include <iostream>
#include <unordered_set>
#include <list>

#include "input.h"

class CDay4Card
{
    const int *m_numbers = nullptr;
    bool m_marked[5][5];

public:
    CDay4Card()
    {
        memset(m_marked, 0, sizeof(m_marked));
    }

    CDay4Card(const int *numbers)
        : m_numbers(numbers)
    {
        memset(m_marked, 0, sizeof(m_marked));
    }

    int NumberAt(int row, int col) const { return m_numbers[col + row * 5]; }

    bool CheckRow(int row) const
    {
        for(int i = 0; i < 5; ++i)
        {
            if(!m_marked[row][i])
                return false;
        }
        return true;
    }

    bool CheckCol(int col) const
    {
        for(int i = 0; i < 5; ++i)
        {
            if(!m_marked[i][col])
                return false;
        }
        return true;
    }

    bool Pick(int number)
    {
        for(int row = 0; row < 5; ++row)
        {
            for(int col = 0; col < 5; ++col)
            {
                if(NumberAt(row, col) == number)
                {
                    // mark the number
                    m_marked[row][col] = true;

                    // is full row marked?
                    if(CheckRow(row))
                        return true;

                    // is full column marked 
                    if(CheckCol(col))
                        return true;

                    return false;
                }
            }
        }

        return false;
    }

    int SumUnmarked() const
    {
        int sum = 0;
        for(int row = 0; row < 5; ++row)
        {
            for(int col = 0; col < 5; ++col)
            {
                if(!m_marked[row][col])
                {
                    sum += NumberAt(row, col);
                }
            }
        }
        return sum;
    }
};

void day4_bingo()
{
    // Initialze cards
    std::list<CDay4Card> cards;
    for(int i = 0; i < _countof(g_day4_cards); i += 25)
    {
        cards.emplace_back(g_day4_cards + i);
    }

    CDay4Card last_winner;
    int last_winning_number = 0;

    for(int number : g_day4_numbers)
    {
        for(auto it = cards.begin(); it != cards.end();)
        {
            auto &card = *it;
            if(card.Pick(number))
            {
                last_winner = card;
                last_winning_number = number;

                // remove the card from play
                it = cards.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    if (last_winning_number > 0)
    {
        int sum_unmarked = last_winner.SumUnmarked();
        int prod = sum_unmarked * last_winning_number;
        std::cout << "score=" << prod << std::endl;
    }
}

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
    day4_bingo();

    return 0;
}
