#include "pch.h"
#include "SolutionFactory.h"
#include "Dijkstras.h"

class CSolution16 : public CSolutionBase
{
public:
    CSolution16() : CSolutionBase()
    {
    }

    struct PathContext
    {
        char Symbol;
        size_t Predecessor;
    };

    struct MazeNode
    {
        const AsciiGrid& MazeGrid;
        size_t Row;
        size_t Col;
        std::vector<size_t> NeighborIndices;

        MazeNode() = default;
        MazeNode(const AsciiGrid& mazeGrid, size_t row, size_t col, char symbol) :
            MazeGrid(mazeGrid),
            Row(row),
            Col(col)
        {
            if (symbol == '#' || symbol == 'E')
                return; // Don't add any neighbor nodes
            
            size_t index = GetIndex();

            if (Row > 0 && MazeGrid.GetValue(row - 1, col) != '#')
            {
                // North
                NeighborIndices.emplace_back(index - MazeGrid.Width());
            }

            if (Row < MazeGrid.Height() && MazeGrid.GetValue(row + 1, col) != '#')
            {
                // South
                NeighborIndices.emplace_back(index + MazeGrid.Width());
            }

            if (Col > 0 && MazeGrid.GetValue(row, col - 1) != '#')
            {
                NeighborIndices.emplace_back(index - 1);
            }

            if (Col < MazeGrid.Width() && MazeGrid.GetValue(row, col + 1) != '#')
            {
                NeighborIndices.emplace_back(index + 1);
            }
        }

        size_t GetIndex() const { return Row* MazeGrid.Width() + Col; }

        size_t NumNeighbors() const
        {
            return NeighborIndices.size();
        }

        size_t NeighborGraphIndex(size_t i) const
        {
            return NeighborIndices[i];
        }

        char MoveToNeighborSymbol(const MazeNode &dest) const
        {
            char moveSymbol = '.';
            if (Row == dest.Row)
            {
                if (Col < dest.Col)
                    moveSymbol = '>';
                else
                    moveSymbol = '<';
            }
            else
            {
                if (Row < dest.Row)
                    moveSymbol = 'v';
                else
                    moveSymbol = '^';
            }

            return moveSymbol;
        }
    };

    struct EdgeCostFunc
    {
        size_t operator()(size_t sourceIndex, size_t destIndex, const MazeNode *nodes, PathContext *pathContexts) const
        {
            size_t cost = 1;

            const MazeNode& dest = nodes[destIndex];
            const MazeNode& source = nodes[sourceIndex];
            char optimalSymbol = source.MoveToNeighborSymbol(dest);

            if (optimalSymbol != pathContexts[sourceIndex].Symbol)
                cost += 1000;

            return cost;
        }
    };

    struct VisitFromFunc
    {
        void operator()(size_t sourceIndex, size_t destIndex, const MazeNode* nodes, size_t* costs, PathContext* pathContexts) const
        {
            const MazeNode& dest = nodes[destIndex];
            const MazeNode& source = nodes[sourceIndex];
            pathContexts[destIndex].Symbol = source.MoveToNeighborSymbol(dest);
            pathContexts[destIndex].Predecessor = sourceIndex;
        }
    };

    AsciiGrid m_MazeGrid;
    std::vector<MazeNode> m_MazeNodes;

    virtual void Execute(int part)
    {
        std::ifstream fstream("2024/Day16.txt");

        m_MazeGrid.ReadFromStream(fstream);
        m_MazeGrid.Print();

        size_t numNodes = m_MazeGrid.Width() * m_MazeGrid.Height();
        std::vector<MazeNode> mazeNodes;
        size_t endIndex;
        size_t startIndex;

        std::vector<size_t> costs(numNodes, std::numeric_limits<size_t>::max());
        std::vector<PathContext> pathContexts(numNodes);

        for (size_t row = 0; row < m_MazeGrid.Height(); ++row)
        {
            for (size_t col = 0; col < m_MazeGrid.Width(); ++col)
            {
                size_t index = row * m_MazeGrid.Width() + col;
                auto symbol = m_MazeGrid.GetValue(row, col);

                if (symbol == 'E')
                {
                    endIndex = index;
                }
                else if (symbol == 'S')
                {
                    startIndex = index;
                    symbol = '>';
                }

                mazeNodes.emplace_back(m_MazeGrid, row, col, symbol == 'S' ? '>' : symbol);

                pathContexts[index].Symbol = symbol;
            }
        }

        if (part == 1)
        {
            size_t minCost = Dijkstras::Execute<MazeNode, size_t, PathContext, EdgeCostFunc, VisitFromFunc>(mazeNodes.size(), mazeNodes.data(), costs.data(), pathContexts.data(), startIndex, endIndex);

            assert(minCost != std::numeric_limits<size_t>::max());

            // Print all paths
            for (size_t index = 0; index < mazeNodes.size(); ++index)
            {
                if (index % m_MazeGrid.Width() == 0)
                    std::cout << std::endl;
                
                std::cout << pathContexts[index].Symbol;
            }
            std::cout << std::endl;

            // Walk from the end to the start to build the path coordinates
            std::deque<size_t> path;
            for (size_t index = endIndex; index != startIndex; index = pathContexts[index].Predecessor)
            {
                path.push_front(index);
            }

            // Print the final path
            auto pathGrid = m_MazeGrid;
            for (auto index : path)
            {
                pathGrid.SetValue(mazeNodes[index].Row, mazeNodes[index].Col, pathContexts[index].Symbol);
            }

            pathGrid.Print();

            std::cout << "Part 1: Score: " << minCost << std::endl;
        }
        else
        {
            size_t minCost = Dijkstras::Execute<MazeNode, size_t, PathContext, EdgeCostFunc, VisitFromFunc>(mazeNodes.size(), mazeNodes.data(), costs.data(), pathContexts.data(), startIndex, endIndex);

            // For each location not already on the original path,
            // calculate the additional cost to the end. If it matches
            // the minimum cost, then add the new path to as an alternative
            // route.
            std::set<size_t> uniqueVisits;
            for (size_t index = endIndex; index != startIndex; index = pathContexts[index].Predecessor)
            {
                uniqueVisits.insert(index);
            }
            uniqueVisits.insert(startIndex);

            for (size_t altIndex = 0; altIndex < mazeNodes.size(); ++altIndex)
            {
                if (costs[altIndex] > minCost)
                    continue; // Already too expensive
                std::vector<size_t> altCosts(mazeNodes.size(), std::numeric_limits<size_t>::max());
                std::vector<PathContext> altPathContexts(mazeNodes.size());
                altPathContexts[altIndex] = pathContexts[altIndex]; // Restore the original path context for the start node
                size_t maxCost = minCost - costs[altIndex];
                size_t additionalCost = Dijkstras::Execute<MazeNode, size_t, PathContext, EdgeCostFunc, VisitFromFunc>(mazeNodes.size(), mazeNodes.data(), altCosts.data(), altPathContexts.data(), altIndex, endIndex, maxCost);
                if (additionalCost > 0)
                {
                    size_t altCost = costs[altIndex] + additionalCost;

                    if (altCost == minCost && uniqueVisits.find(altCost) == uniqueVisits.end())
                    {
                        for (size_t index = endIndex; index != altIndex; index = altPathContexts[index].Predecessor)
                        {
                            uniqueVisits.insert(index);
                        }
                        uniqueVisits.insert(altIndex);
                    }
                }
            }

            AsciiGrid tempGrid = m_MazeGrid;

            for (auto index : uniqueVisits)
            {
                tempGrid.SetValue(mazeNodes[index].Row, mazeNodes[index].Col, 'O');
            }

            tempGrid.Print();

            assert(minCost != std::numeric_limits<size_t>::max());
            std::cout << "Part 2: Score: " << uniqueVisits.size() << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution16, 16, "Reindeer Maze");