
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

#include "day15.h"

const auto MAX_SIZE_T = std::numeric_limits<size_t>::max();

namespace Day15
{
	size_t CoordToIndex(int maxWidth, int coord[2])
	{
		return maxWidth * coord[1] + coord[0];
	}

	void IndexToCoord(int maxWidth, size_t index, int coord[2])
	{
		int y = int(index / maxWidth);
		int x = int(index - y * maxWidth);
		coord[0] = x;
		coord[1] = y;
	}

	struct Node
	{
		int NumNeighbors = 0;
		size_t Neighbors[4] = {};
	};

	class Chiton
	{ 
		std::vector<Node> m_Nodes;
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
		std::priority_queue<Edge, std::vector<Edge>, Edge::Greater> m_EdgeQueue;

		static int CavernWidth() { return 5 * g_CavernWidth; }
		static int CavernLength() { return 5 * g_CavernLength; }
		static size_t CavernSize() { return CavernWidth() * CavernLength(); }

		static int Risk(size_t index)
		{
			int coord[2];
			IndexToCoord(CavernWidth(), index, coord);
			int SourceCoord[2];
			SourceCoord[0] = coord[0] % g_CavernWidth;
			SourceCoord[1] = coord[1] % g_CavernLength;
			size_t SourceIndex = CoordToIndex(g_CavernWidth, SourceCoord);
			int SourceRisk = g_Risk[SourceIndex];
			int RiskInc = coord[0] / g_CavernWidth + coord[1] / g_CavernLength;
			return (((SourceRisk + RiskInc) - 1) % 9) + 1;
		}

	public:
		Chiton()
		{
			// Build node graph
			for(size_t i = 0; i < CavernSize(); ++i)
			{
				Node &node = m_Nodes.emplace_back();
				int coord[2];
				IndexToCoord(CavernWidth(), i, coord);
				if(coord[0] > 0)
				{ 
					int neighborCoord[2] = { coord[0] - 1, coord[1] };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(CavernWidth(), neighborCoord);
					node.NumNeighbors++;
				}
				if(coord[0] < CavernWidth() - 1) 
				{
					int neighborCoord[2] = { coord[0] + 1, coord[1] };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(CavernWidth(), neighborCoord);
					node.NumNeighbors++;
				}
				if(coord[1] > 0)
				{
					int neighborCoord[2] = { coord[0], coord[1] - 1 };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(CavernWidth(), neighborCoord);
					node.NumNeighbors++;
				}
				if(coord[1] < CavernLength() - 1)
				{
					int neighborCoord[2] = { coord[0], coord[1] + 1 };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(CavernWidth(), neighborCoord);
					node.NumNeighbors++;
				}
				//if (i > 0)
				//{
				//	if (0 == (coord[0] % CavernWidth()))
				//	{
				//		std::cout << std::endl;

				//		if (0 == coord[1] % g_CavernLength)
				//			std::cout << std::endl;
				//	}
				//	else
				//	{
				//		if (0 == (coord[0] % g_CavernWidth))
				//		{
				//			std::cout << " ";
				//		}
				//	}
				//}

				//std::cout << Risk(i);
			}
			//std::cout << std::endl;
		}

		size_t Dijkstras(size_t startIndex, size_t endIndex)
		{
			// Add the start node cost to the priority queue
			m_Visited.resize(CavernSize(), false);
			m_Costs.resize(CavernSize(), MAX_SIZE_T);
			m_Costs[startIndex] = 0;
			m_EdgeQueue.emplace(0, startIndex);

			while (!m_EdgeQueue.empty())
			{
				Edge edge = m_EdgeQueue.top();
				m_EdgeQueue.pop();
				size_t Index = edge.DestIndex;
				if (m_Visited[Index]) continue;
				m_Visited[Index] = true;
				if (Index == endIndex)
				{
					return m_Costs[Index];
				}
				const Node& node = m_Nodes[Index];

				for (int i = 0; i < node.NumNeighbors; ++i)
				{
					size_t NeighborIndex = node.Neighbors[i];
					size_t NewCost = m_Costs[Index] + Risk(NeighborIndex);
					if (NewCost < m_Costs[NeighborIndex])
					{
						m_Costs[NeighborIndex] = NewCost;
						m_EdgeQueue.emplace(NewCost, NeighborIndex);
					}
				}
			}

			return MAX_SIZE_T;
		}

		void Execute()
		{

			size_t StartIndex = 0;
			size_t EndIndex = CavernSize() - 1;

			size_t Cost = Dijkstras(StartIndex, EndIndex);

			std::cout << "Min cost=" << Cost << std::endl;
		}
	};

    void Execute()
    {
        Day15::Chiton puzzle;
        puzzle.Execute();
    }
}

