#pragma once

#include <vector>
#include <queue>

class Dijkstras
{
    static const auto MaxCost = std::numeric_limits<size_t>::max();

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
    Dijkstras(size_t NumNodes) :
        m_Visited(NumNodes),
        m_Costs(NumNodes) {}

    // Templatized Execute function requires that `NodeType` implement the following:
    // size_t NumNeighbors()
    // size_t NeighborIndex(size_t i) // Where `i` is in the range `[0, NumNeighbors())`
    //
    // In addition, the functor EdgeCostFunc must implement:
    // operator()(const NodeType &node, const NodeType &neighbor)
    // Where `neighbor` is one of the neighbors of `node`.
    template<class NodeType, class EdgeCostFunc>
    size_t Execute(const std::vector<NodeType> &Nodes, size_t startIndex, size_t endIndex)
    {
        // Initialize data
        std::fill(m_Visited.begin(), m_Visited.end(), false);
        std::fill(m_Costs.begin(), m_Costs.end(), MaxCost);
        m_Costs[startIndex] = 0;

        // Add the start node cost to the priority queue
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
            const NodeType& node = Nodes[Index];

            for (int i = 0; i < node.NumNeighbors(); ++i)
            {
                size_t NeighborIndex = node.NeighborIndex(i);
                EdgeCostFunc EdgeCost;
                size_t NewCost = m_Costs[Index] + EdgeCost(node, Nodes[NeighborIndex]);
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
