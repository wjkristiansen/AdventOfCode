#pragma once

#include <vector>
#include <queue>

class Dijkstras
{
    static const auto MaxCost = std::numeric_limits<size_t>::max();

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
    template<class NodeType>
    struct DefaultEdgeCost
    {
        size_t operator()(const std::vector<NodeType>&, size_t, size_t)
        {
            // By default, assume the cost traversing to any neighbor
            // has a cost of 1.
            return 1;
        }
    };

    template<class NodeType>
    struct DefaultUpdatePredecessor
    {
        void operator()(std::vector<NodeType>&, size_t, size_t)
        {
            // Do nothing by default
        }
    };

    // Templatized Execute function requires that `NodeType` implement the following:
    // size_t NumNeighbors()
    // size_t NeighborIndex(size_t i) // Where `i` is in the range `[0, NumNeighbors())`
    //
    // The user may provide a custom EdgeCostFunc in the template parameters.
    // The EdgeCostFunc functor must implement
    // `size_t operator()(const std::vector<NodeType>& nodes, size_t sourceIndex, size_t destIndex)`
    //
    // Additionally, the user may provide a custom UpdatePredecessorFunc in the template parameters.
    // The UpdatePredecessorFunc functor must implement
    // `void operator()(std::vector<NodeType>& nodes, size_t sourceIndex, size_t destIndex)`
    // The purpose of the UpdatePredecessorFunc is to optionally track node state. This can be
    // useful for recording evaluated paths or for dynamic costing.
    template<class NodeType, class EdgeCostFunc = DefaultEdgeCost<NodeType>, class UpdatePredecessorFunc = DefaultUpdatePredecessor<NodeType>>
    static size_t Execute(std::vector<NodeType>& nodes, size_t startIndex, size_t endIndex)
    {
        std::vector<bool> Visited(nodes.size(), false);
        std::vector<size_t> Costs(nodes.size(), MaxCost);
        Costs[startIndex] = 0;

        // Add the start node cost to the priority queue
        std::priority_queue<Edge, std::vector<Edge>, Edge::Greater> EdgeQueue;
        EdgeQueue.emplace(0, startIndex);

        while (!EdgeQueue.empty())
        {
            Edge edge = EdgeQueue.top();
            EdgeQueue.pop();
            size_t Index = edge.DestIndex;

            if (Visited[Index])
                continue;

            Visited[Index] = true;
            if (Index == endIndex)
            {
                return edge.Cost;
            }

            const NodeType& node = nodes[Index];

            for (int i = 0; i < node.NumNeighbors(); ++i)
            {
                size_t NeighborIndex = node.NeighborIndex(i);
                size_t NewCost = edge.Cost + EdgeCostFunc()(nodes, Index, NeighborIndex);
                if (NewCost < Costs[NeighborIndex])
                {
                    Costs[NeighborIndex] = NewCost;
                    EdgeQueue.emplace(NewCost, NeighborIndex);
                    UpdatePredecessorFunc()(nodes, Index, NeighborIndex);
                }
            }
        }

        return MaxCost;
    }
};
