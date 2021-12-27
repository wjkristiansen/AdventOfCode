#include <iostream>
#include <unordered_set>
#include <list>
#include <deque>
#include <unordered_map>
#include <algorithm>

#include <assert.h>

#include "input.h"

namespace Day14
{
    class Polymerization
    {
        static uint16_t AsPairKey(const char* pair)
        {
            return *(reinterpret_cast<const uint16_t*>(pair));
        }

        static size_t AsCounterKey(uint16_t pairKey, uint16_t level)
        {
            return pairKey + level * 65536;
        }

        struct Counter
        {
            uint64_t Counts[32] = { 0 };
        };
        std::unordered_map<size_t, Counter> m_counts;
        std::unordered_map<uint16_t, char> m_rules;
        const int m_maxDepth = 40;

    public:
        Polymerization()
        {
            for (const char *rule : g_rules)
            {
                uint16_t ruleKey = AsPairKey(rule);
                m_rules.emplace(ruleKey, rule[4]);
            }
        }

        const Counter &Expand(int depth, const char* pChars)
        {
            uint16_t level = uint16_t(depth);
            size_t counterKey = AsCounterKey(AsPairKey(pChars), level);

            auto it = m_counts.find(counterKey);
            if (it != m_counts.end())
                return it->second;

            auto result = m_counts.emplace(counterKey, Counter());
            it = result.first;

            Counter& counter = it->second;

            if (depth < m_maxDepth)
            {
                uint16_t ruleKey = AsPairKey(pChars);
                char next[3];
                next[0] = pChars[0];
                next[1] = m_rules[ruleKey];
                next[2] = pChars[1];
                const Counter &left = Expand(depth + 1, &next[0]);
                const Counter& right = Expand(depth + 1, &next[1]);

                for (size_t accIndex = 0; accIndex < 32; ++accIndex)
                {
                    counter.Counts[accIndex] += left.Counts[accIndex] + right.Counts[accIndex];
                }
            }
            else
            {
                size_t index = pChars[0] - 'A';
                counter.Counts[index]++;
            }

            return counter;
        }

        void Execute()
        {
            Counter rootCounter;
            const int lastIndex = sizeof(g_template) - 2;
            for (int i = 0; i < lastIndex; ++i)
            {
                // Decode rules using depth-first and count elements
                const Counter &left = Expand(0, &g_template[i]);
                for (size_t accIndex = 0; accIndex < 32; ++accIndex)
                {
                    rootCounter.Counts[accIndex] += left.Counts[accIndex];
                }
            }

            // Count the last element
            rootCounter.Counts[g_template[lastIndex] - 'A']++;
//            std::cout << g_template[lastIndex] << std::endl;

            uint64_t most = 0;
            uint64_t least = UINT64_MAX;
            char mostLetter = 0;
            char leastLetter = 0;
            for (char index = 0; index < 32; ++index)
            {
                uint64_t count = rootCounter.Counts[index];
                if (count > 0)
                {
                    if (count < least)
                    {
                        least = count;
                        leastLetter = index + 'A';
                    }

                    if (count > most)
                    {
                        most = count;
                        mostLetter = index + 'A';
                    }
                }
            }

            std::cout << "Most=" << most << "(" << mostLetter << ")" << std::endl;
            std::cout << "Least=" << least << "(" << leastLetter << ")" << std::endl;
            std::cout << "Delta=" << most - least << std::endl;
        }
    };
}

namespace Day13
{
    class Origami
    {
        struct Point
        {
            int x, y;

            Point(int X, int Y) :
                x(X), y(Y) {};

            bool operator==(const Point& o) const
            {
                return x == o.x && y == o.y;
            }
            bool operator!=(const Point& o) const
            {
                return !operator==(o);
            }
        };

        struct PointHash
        {
            std::size_t operator()(const Point& p) const
            {
                return size_t(p.x + p.y * 2048);
            }
        };

        std::unordered_set<Point, PointHash> m_points;
        int m_Width = 0;
        int m_Height = 0;

    public:
        Origami()
        {
            for (auto point : g_points)
            {
                m_points.emplace(point[0], point[1]);
            }
        }

        Point Reflect(FoldAxis axis, int loc, const Point p)
        {
            Point result = p;
            switch (axis)
            {
            case FoldAxis::x:
                m_Width = loc;
                if (result.x > loc)
                    result.x = loc * 2 - result.x;
                break;

            case FoldAxis::y:
                m_Height = loc;
                if(result.y > loc)
                    result.y = loc * 2 - result.y;
                break;
            }

            return result;
        }

        void Fold(FoldAxis axis, int loc)
        {
            std::unordered_set<Point, PointHash> temp;
            
            // Add fold results to temp set and remove folded points
            for (auto it = m_points.begin(); it != m_points.end(); )
            {
                const Point& point = *it;
                Point p = Reflect(axis, loc, point);
                if (p != point)
                {
                    temp.emplace(p);
                    it = m_points.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // Combine fold results with existing set
            m_points.merge(temp);
        }

        void Execute()
        {
            for (Day13::Fold f : g_folds)
            {
                Fold(f.axis, f.location);
                std::cout << "Visible=" << m_points.size() << std::endl;
            }

            for (auto row = 0; row < m_Height; ++row)
            {
                for (auto col = 0; col < m_Width; ++col)
                {
                    auto it = m_points.find(Point(col, row));
                    if (it == m_points.end())
                        std::cout << '.';
                    else
                        std::cout << '#';
                }
                std::cout << std::endl;
            }
        }
    };
}

namespace Day12
{
    struct CaveNode
    {
        int VisitCount = 0;
        bool IsLarge = false;
        std::string Name;
        std::vector<int> NeighborIndices;

        CaveNode(const std::string& name) :
            Name(name)
        {
            IsLarge = name[0] >= 'A' && name[0] <= 'Z';
        }
    };

    class PassagePathing
    {
    public:

        std::vector<CaveNode> m_caveNodes;
        std::unordered_map<std::string, int> m_indexMap;
        int m_startIndex = 0;
        int m_endIndex = 0;
        int m_pathCount = 0;
        int m_smallExtra = -1;

        int GetCaveNodeIndex(const std::string& name)
        {
            auto it = m_indexMap.find(name);
            if (it == m_indexMap.end())
            {
                // Add cave node
                int index = static_cast<int>(m_caveNodes.size());
                m_caveNodes.emplace_back(name);
                auto insert = m_indexMap.emplace(name, index);
                it = insert.first;

                if (name == "start")
                {
                    m_startIndex = index;
                }

                if (name == "end")
                {
                    m_endIndex = index;
                }
            }

            return it->second;
        }

        void ParseInput(const char* strings[], int numStrings)
        {
            for (int i = 0; i < numStrings; ++i)
            {
                std::string line(strings[i]);
                int delim = line.find('-');
                std::string name = line.substr(0, delim);
                int index1 = GetCaveNodeIndex(name);
                name = line.substr(delim + 1);
                int index2 = GetCaveNodeIndex(name);
                m_caveNodes[index1].NeighborIndices.push_back(index2);
                m_caveNodes[index2].NeighborIndices.push_back(index1);
            }
        }

        void CalcPaths(int nodeIndex, std::deque<int>& pathQueue)
        {
            CaveNode& node = m_caveNodes[nodeIndex];

            if (!node.IsLarge)
            {
                if (node.VisitCount > 0)
                {
                    if (m_smallExtra < 0 && nodeIndex != m_startIndex && nodeIndex != m_endIndex)
                        m_smallExtra = nodeIndex;
                    else
                        return;
                }

                if (node.VisitCount > 1 && m_smallExtra != nodeIndex)
                    return;
            }

            pathQueue.push_back(nodeIndex);
            node.VisitCount++;
            for (int neighborIndex : node.NeighborIndices)
            {
                if (neighborIndex == m_endIndex)
                {
                    //for (auto segment : pathQueue)
                    //{
                    //    std::cout << m_caveNodes[segment].Name << ",";
                    //}
                    //std::cout << "end" << std::endl;

                    m_pathCount++;
                    continue;
                }

                CalcPaths(neighborIndex, pathQueue);
            }
            if (m_smallExtra == nodeIndex)
            {
                m_smallExtra = -1;
            }
            node.VisitCount--;
            pathQueue.pop_back();
        }

        void Execute()
        {
            ParseInput(g_passages, _countof(g_passages));

            std::deque<int> pathQueue;
            CalcPaths(m_startIndex, pathQueue);

            std::cout << "NumPaths=" << m_pathCount << std::endl;
        }
    };
};

int main(int argc, char *argv[])
{
    Day14::Polymerization puzzle;
    puzzle.Execute();

    return 0;
}
