#pragma once

#include <vector>
#include <queue>

class Dijkstras
{
    template<class CostType>
    struct Edge
    {
        CostType Cost;
        size_t DestIndex;

        Edge(CostType cost, size_t destIndex) :
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

    struct DefaultPathContext {};

    template<class NodeType, class CostType, class PathContextType>
    struct DefaultEdgeCostFunc
    {
        CostType operator()(size_t sourceIndex, size_t destIndex, const NodeType* nodes, PathContextType* pathContexts)
        {
            return 1;
        }
    };

    template<class NodeType, class CostType, class PathContextType>
    struct DefaultVisitFromFunc
    {
        void operator()(size_t sourceIndex, size_t destIndex, const NodeType* nodes, CostType* costs, PathContextType* pathContexts)
        {
            // Do nothing
        }
    };

public:
    // Templatized Execute function requires that `NodeType` implement the following:
    // size_t NumNeighbors()
    // size_t NeighborGraphIndex(size_t i) // Where `i` is in the range `[0, NumNeighbors())`
    // size_t EdgeCost(size_t sourceIndex, size_t destIndex, const NodeType *nodes) const // Assumes dest is a neighbor to this node
    // void VisitFrom(size_t sourceIndex, size_t destIndex, const NodeType *nodes, CostType *costs, PathContextType *pathContexts) const
    //
    // The `CostType` template parameter is the costing value type.
    //
    // The `PathContext` template parameter is a type used to keep track of 
    // user-specific transient data for a given Dijkstras::Execute instance.
    // This can be used to keep track of path state such as direction of travel
    // or keeping track of predecessor nodes.
    template<
        class NodeType,
        class CostType = size_t,
        class PathContextType = DefaultPathContext,
        class EdgeCostFunc = DefaultEdgeCostFunc<NodeType, CostType, PathContextType>,
        class VisitFromFunc = DefaultVisitFromFunc<NodeType, CostType, PathContextType>>
    static CostType Execute(size_t numNodes, const NodeType* nodes, CostType* costs, PathContextType* pathContexts, size_t startIndex, size_t endIndex, CostType maxCost = 0)
    {
        CostType cost = 0;
        costs[startIndex] = cost;

        // Add the start node cost to the priority queue
        std::priority_queue<Edge<CostType>, std::vector<Edge<CostType>>, typename Edge<CostType>::Greater> EdgeQueue;
        EdgeQueue.emplace(0, startIndex);

        while (!EdgeQueue.empty())
        {
            Edge<CostType> edge = EdgeQueue.top();
            EdgeQueue.pop();
            size_t index = edge.DestIndex;

            if (startIndex != endIndex && index == endIndex)
            {
                cost = costs[index];
                break;
            }

            if (edge.Cost > costs[index])
                continue;

            if (maxCost != 0 && edge.Cost > maxCost)
                continue;

            const NodeType& source = nodes[index];

            for (int i = 0; i < source.NumNeighbors(); ++i)
            {
                size_t destIndex = source.NeighborGraphIndex(i);

                if (edge.Cost >= costs[destIndex])
                    continue;

                const NodeType& dest = nodes[destIndex];
                CostType NewCost = edge.Cost + EdgeCostFunc()(index, destIndex, nodes, pathContexts);
                if (NewCost < costs[destIndex])
                {
                    costs[destIndex] = NewCost;
                    EdgeQueue.emplace(NewCost, destIndex);
                    VisitFromFunc()(index, destIndex, nodes, costs, pathContexts);
                }
            }
        }

        return cost;
    }
};
