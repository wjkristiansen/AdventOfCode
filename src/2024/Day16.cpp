#include "pch.h"
#include "SolutionFactory.h"
#include "Dijkstras.h"

class CSolution16 : public CSolutionBase
{
public:
    CSolution16() : CSolutionBase()
    {
    }

    struct MazeNode
    {
        const AsciiGrid& MazeGrid;
        size_t Row;
        size_t Col;
        size_t PredecessorIndex;
        char Symbol;
        std::vector<size_t> NeighborIndices;

        MazeNode() = default;
        MazeNode(const AsciiGrid& mazeGrid, size_t row, size_t col) :
            MazeGrid(mazeGrid),
            Row(row),
            Col(col),
            PredecessorIndex(NodeIndex()),
            Symbol(mazeGrid.GetValue(row, col))
        {
            std::set<size_t> tempNeighborIndices
            {
                {NodeIndex() + 1}, // East
                {NodeIndex() + MazeGrid.Width()}, // South
                {NodeIndex() - 1}, // West
                {NodeIndex() - MazeGrid.Width()}, // North
            };

            if (Row == 0)
            {
                tempNeighborIndices.erase(NodeIndex() - MazeGrid.Width()); // No North
            }
            if (Row == MazeGrid.Height() - 1)
            {
                tempNeighborIndices.erase(NodeIndex() + MazeGrid.Width()); // No South
            }
            if (Col == 0)
            {
                tempNeighborIndices.erase(NodeIndex() - 1); // No West
            }
            if (Col == MazeGrid.Width() - 1)
            {
                tempNeighborIndices.erase(NodeIndex() + 1); // No East
            }

            // See if any of the remaining nodes are blocked
            for (auto it = tempNeighborIndices.begin(); it != tempNeighborIndices.end();)
            {
                size_t index = *it;
                size_t row = index / MazeGrid.Width();
                size_t col = index % MazeGrid.Width();

                if (MazeGrid.GetValue(row, col) == '#')
                    it = tempNeighborIndices.erase(it);
                else
                    ++it;
            }

            NeighborIndices = std::vector<size_t>(tempNeighborIndices.begin(), tempNeighborIndices.end());
        }

        size_t NodeIndex() const
        {
            return Row * MazeGrid.Height() + Col;
        };

        size_t NumNeighbors() const
        {
            return NeighborIndices.size();
        }

        size_t NeighborIndex(size_t i) const
        {
            return NeighborIndices[i];
        }
    };

    struct EdgeCost
    {
        size_t operator()(const std::vector<MazeNode> &nodes, size_t sourceIndex, size_t destIndex)
        {
            const MazeNode& source = nodes[sourceIndex];
            const MazeNode& dest = nodes[destIndex];
            char sourceDirSymbol = source.Symbol;
            char newDirSymbol = '.';
            if (source.Col == dest.Col)
            {
                newDirSymbol = source.Row < dest.Row ? 'v' : '^';
            }
            else
            {
                assert(source.Row == dest.Row);
                newDirSymbol = source.Col < dest.Col ? '>' : '<';
            }

            if (newDirSymbol != sourceDirSymbol)
            {
                return 1001;
            }

            return 1;
        }
    };

    struct UpdatePredecessor
    {
        void operator()(std::vector<MazeNode>& nodes, size_t sourceIndex, size_t destIndex)
        {
            const MazeNode& source = nodes[sourceIndex];
            MazeNode& dest = nodes[destIndex];

            dest.PredecessorIndex = sourceIndex;
            char sourceDirSymbol = source.Symbol;
            char newDirSymbol = '.';
            if (source.Col == dest.Col)
            {
                newDirSymbol = source.Row < dest.Row ? 'v' : '^';
            }
            else
            {
                assert(source.Row == dest.Row);
                newDirSymbol = source.Col < dest.Col ? '>' : '<';
            }
            dest.Symbol = newDirSymbol;
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

        for (size_t row = 0; row < m_MazeGrid.Height(); ++row)
        {
            for (size_t col = 0; col < m_MazeGrid.Width(); ++col)
            {
                mazeNodes.emplace_back(m_MazeGrid, row, col);
            }
        }

        size_t startIndex = 0;
        size_t endIndex = 0;
        for (size_t index = 0; index < mazeNodes.size(); ++index)
        {
            auto& node = mazeNodes[index];

            switch (node.Symbol)
            {
            case 'S':
                node.Symbol = '>';
                startIndex = index;
                break;
            case 'E':
                endIndex = index;
                break;
            }
        }

        size_t minCost = Dijkstras::Execute<MazeNode, EdgeCost, UpdatePredecessor>(mazeNodes, startIndex, endIndex);

        assert(minCost != std::numeric_limits<size_t>::max());

        // Walk from the end to the start to build the path coordinates
        std::deque<size_t> path;
        for (size_t index = endIndex; index != startIndex; index = mazeNodes[index].PredecessorIndex)
        {
            path.push_front(index);
        }

        // Print the final path
        auto pathGrid = m_MazeGrid;
        for (auto index : path)
        {
            pathGrid.SetValue(mazeNodes[index].Row, mazeNodes[index].Col, mazeNodes[index].Symbol);
        }

        pathGrid.Print();

        std::cout << "Part 1: Score: " << minCost << std::endl;
    }
};

DECLARE_SOLUTION(CSolution16, 16, "Reindeer Maze");