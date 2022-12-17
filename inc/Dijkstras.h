#pragma once

#include <vector>
#include <queue>

template<class NodeType>
class Dijkstras
{
    static const auto MaxCost = std::numeric_limits<size_t>::max();

    std::vector<NodeType> &m_Nodes;
    std::vector<bool> m_Visited;
    std::vector<size_t> m_Costs;

    struct Edge
    {
        size_t Cost;
        size_t DestIndex;

        Edge(size_t cost, size_t destIndex) :
            Cost(cost),
            DestIndex(destIndex) {}

        struct Greater
        {
            bool operator()(const Edge& e1, const Edge& e2)
            {
                return e1.Cost > e2.Cost;
            }
        };
    };

public:
    Dijkstras(const std::vector<NodeType> &Nodes) :
        m_Nodes(Nodes),
        m_Visited(Nodes.size()),
        m_Costs(Nodes.size()) {}

    size_t Execute(size_t startIndex, size_t endIndex)
    {
        // Add the start node cost to the priority queue
        std::fill(m_Visited.begin(), m_Visited.end(), false);
        std::fill(m_Costs.begin(), m_Visited.end(), MaxCost);
        m_Costs[startIndex] = 0;

        std::priority_queue<Edge, std::vector<Edge>, Edge::Greater> EdgeQueue;
        EdgeQueue.emplace(0, startIndex);

        while (!EdgeQueue.empty())
        {
            Edge edge = EdgeQueue.top();
            EdgeQueue.pop();
            size_t Index = edge.DestIndex;

            if (m_Visited[Index])
                continue;
            
            m_Visited[Index] = true;
            if (Index == endIndex)
            {
                return m_Costs[Index];
            }
            const NodeType& node = m_Nodes[Index];

            for (int i = 0; i < node.NumNeighbors(); ++i)
            {
                size_t NeighborIndex = node.NeighborIndex(i);
                size_t NewCost = m_Costs[Index] + node.Cost(NeighborIndex);
                if (NewCost < m_Costs[NeighborIndex])
                {
                    m_Costs[NeighborIndex] = NewCost;
                    EdgeQueue.emplace(NewCost, NeighborIndex);
                }
            }
        }

        return MaxCost;
    }
};
