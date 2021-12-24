#include <iostream>
#include <unordered_set>
#include <list>
#include <deque>
#include <unordered_map>
#include <algorithm>

#include <assert.h>

#include "input.h"

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
    Day12::PassagePathing passagePathing;
    passagePathing.Execute();

    return 0;
}
