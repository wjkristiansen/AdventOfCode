#include "pch.h"
#include "SolutionFactory.h"

#include <Dijkstras.h>

class CSolution12: public CSolutionBase
{
    struct PathContext {};

    struct HeightMapNode
    {
        char Height;
        std::vector<size_t> NeighborIndices;

        HeightMapNode(char height) : Height(height) {}

        size_t NumNeighbors() const { return NeighborIndices.size(); }
        size_t NeighborGraphIndex(size_t i) const { return NeighborIndices[i]; }
    };

    void Execute(int)
    {
        std::ifstream fstream("2022/Day12.txt");

        std::vector<HeightMapNode> HeightMapNodes;
        std::vector<size_t> PotentialStartPoints;
        size_t StartIndex = 0;
        size_t EndIndex = 0;

        size_t MapWidth = 0;
        size_t MapHeight = 0;
        for(;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);
            if(line.empty())
                break;
            for(char c : line)
            {
                if (c == 'S')
                {
                    StartIndex = HeightMapNodes.size();
                    c = 'a';
                }
                else if (c == 'E')
                {
                    EndIndex = HeightMapNodes.size();
                    c = 'z';
                }

                if(c == 'a')
                {
                    PotentialStartPoints.push_back(HeightMapNodes.size());
                }
                HeightMapNodes.emplace_back(c);
            }

            if(MapWidth == 0)
                MapWidth = line.length();

            MapHeight++;
        }

        auto IndexToCoord = [&MapWidth](size_t NodeIndex, size_t &GridX, size_t &GridY)
        {
            GridX = NodeIndex % MapWidth;
            GridY = NodeIndex / MapWidth;
        };

        auto CoordToIndex = [&MapWidth, &MapHeight](size_t GridX, size_t GridY) -> size_t
        {
            return MapWidth * GridY + GridX;
        };

        // Initialize neighbor data
        for(size_t i = 0; i < HeightMapNodes.size(); ++i)
        {
            HeightMapNode &Node = HeightMapNodes[i];
            size_t GridX;
            size_t GridY;
            IndexToCoord(i, GridX, GridY);
            size_t NeighborIndex = CoordToIndex(GridX + 1, GridY);
            if ((GridX < MapWidth - 1) && (HeightMapNodes[NeighborIndex].Height <= Node.Height + 1))
                Node.NeighborIndices.push_back(NeighborIndex);

            NeighborIndex = CoordToIndex(GridX - 1, GridY);
            if ((GridX > 0) && (HeightMapNodes[NeighborIndex].Height <= Node.Height + 1))
                Node.NeighborIndices.push_back(NeighborIndex);

            NeighborIndex = CoordToIndex(GridX, GridY + 1);
            if ((GridY < MapHeight - 1) && (HeightMapNodes[NeighborIndex].Height <= Node.Height + 1))
                Node.NeighborIndices.push_back(NeighborIndex);

            NeighborIndex = CoordToIndex(GridX, GridY - 1);
            if ((GridY > 0) && (HeightMapNodes[NeighborIndex].Height <= Node.Height + 1))
                Node.NeighborIndices.push_back(NeighborIndex);
        }

        std::vector<size_t> costs(HeightMapNodes.size(), std::numeric_limits<size_t>::max());

        size_t steps = Dijkstras::Execute<HeightMapNode, size_t, PathContext>(HeightMapNodes.size(), HeightMapNodes.data(), costs.data(), nullptr, StartIndex, EndIndex);
        assert(steps == costs[EndIndex]);

        std::cout << "Steps: " << steps << std::endl;

        size_t MinSteps = std::numeric_limits<size_t>::max();
        for(auto Start : PotentialStartPoints)
        {
            std::fill(costs.begin(), costs.end(), std::numeric_limits<size_t>::max());
            steps = Dijkstras::Execute<HeightMapNode, size_t, PathContext>(HeightMapNodes.size(), HeightMapNodes.data(), costs.data(), nullptr, Start, EndIndex);
            assert(steps == costs[EndIndex]);
            if (steps < MinSteps)
                MinSteps = steps;
        }

        std::cout << "Least Steps: " << MinSteps << std::endl;
    };
};

DECLARE_SOLUTION(CSolution12, 12, "Hill Climbing Algorithm");
