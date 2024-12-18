#include "pch.h"
#include "SolutionFactory.h"

class CSolution11: public CSolutionBase
{
public:
    CSolution11() : CSolutionBase()
    {
    }

    std::list<size_t> m_Stones;
    std::vector<std::unordered_map<size_t, size_t>> m_NewStoneCounts;

    void InitStones(const char* file)
    {
        auto fstream = std::ifstream(file);
        while (!fstream.eof())
        {
            size_t value;
            fstream >> value;
            m_Stones.push_back(value);
        }
    }

    static bool IsEven(size_t v)
    {
        return 0 == (v & 1);
    }

    static int NumDigits(size_t v)
    {
        int result = 0;

        while (v)
        {
            v = v / 10;
            ++result;
        }

        return result;
    }

    std::pair<size_t, size_t> SplitDigits(size_t digits, int numDigits)
    {
        size_t lowerDigits = 0;
        size_t upperDigits = digits;
        size_t lowerScale = 1;
        for (int i = 0; i < numDigits; ++i)
        {
            size_t newUpperDigits = upperDigits / 10;
            size_t remainder = upperDigits - (newUpperDigits * 10);
            upperDigits = newUpperDigits;
            lowerDigits += remainder * lowerScale;
            lowerScale *= 10;
        }
        return std::pair(upperDigits, lowerDigits);
    }

    size_t RecursiveBlink(int recursionCount, size_t stone)
    {
        size_t result = 1;

        if (recursionCount > 0)
        {
            auto it = m_NewStoneCounts[recursionCount].find(stone);
            if (it != m_NewStoneCounts[recursionCount].end())
                return it->second;

            int innerRecursionCount = recursionCount - 1;

            if (stone == 0)
            {
                // Converts to a 1
                result = RecursiveBlink(innerRecursionCount, 1);
            }
            else
            {
                int numDigits = NumDigits(stone);

                if (IsEven(numDigits))
                {
                    auto newStones = SplitDigits(stone, numDigits / 2);

                    result = RecursiveBlink(innerRecursionCount, newStones.first) + RecursiveBlink(innerRecursionCount, newStones.second);
                }
                else
                {
                    result = RecursiveBlink(innerRecursionCount, stone * 2024);
                }
            }
        }

        m_NewStoneCounts[recursionCount].emplace(stone, result);

        return result;
    }

    void Blink()
    {
        for (auto it = m_Stones.begin(); it != m_Stones.end(); ++it)
        {
            if (*it == 0)
            {
                *it = 1;
            }
            else
            {
                int numDigits = NumDigits(*it);

                if (IsEven(numDigits))
                {
                    auto newStones = SplitDigits(*it, numDigits / 2);

                    m_Stones.insert(it, newStones.first);
                    *it = newStones.second;
                }
                else
                {
                    size_t v = *it * 2024;
                    *it = v;
                }
            }
        }
    }

    void PrintStones()
    {
        auto it = m_Stones.begin();
        for (;;)
        {
            auto stone = *it;
            std::cout << stone;
            ++it;
            if (it == m_Stones.end())
                break;
            std::cout << " ";
        }

        std::cout << std::endl;
    }

    virtual void Execute(int part)
    {
        if (part == 1)
        {
            InitStones("2024/Day11.txt");

            for (int count = 0; count < 25; ++count)
            {
                Blink();
            }

            std::cout << "Number of stones: " << m_Stones.size() << std::endl;
        }
        else
        {
            InitStones("2024/Day11.txt");

            size_t count = 0;
            const int recursionCount = 75;
            m_NewStoneCounts.resize(recursionCount + 1);
            for (auto stone : m_Stones)
            {
                count += RecursiveBlink(recursionCount, stone);
            }

            std::cout << "Number of stones: " << count << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution11, 11, "Plutonian Pebbles");
