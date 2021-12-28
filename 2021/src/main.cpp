#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <unordered_set>

#include <assert.h>

#include "input.h"

const auto MAX_SIZE_T = std::numeric_limits<size_t>::max();

namespace Day15
{
	size_t CoordToIndex(int coord[2])
	{
		return g_CavernWidth * coord[1] + coord[0];
	}

	void IndexToCoord(size_t index, int coord[2])
	{
		int y = int(index / g_CavernWidth);
		int x = int(index - y * g_CavernWidth);
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

	public:
		Chiton()
		{
			// Build node graph
			for(size_t i = 0; i < g_CavernSize; ++i)
			{
				Node &node = m_Nodes.emplace_back();
				int coord[2];
				IndexToCoord(i, coord);
				if(coord[0] > 0)
				{
					int neighborCoord[2] = { coord[0] - 1, coord[1] };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(neighborCoord);
					node.NumNeighbors++;
				}
				if(coord[0] < g_CavernWidth - 1)
				{
					int neighborCoord[2] = { coord[0] + 1, coord[1] };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(neighborCoord);
					node.NumNeighbors++;
				}
				if(coord[1] > 0)
				{
					int neighborCoord[2] = { coord[0], coord[1] - 1 };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(neighborCoord);
					node.NumNeighbors++;
				}
				if(coord[1] < g_CavernLength - 1)
				{
					int neighborCoord[2] = { coord[0], coord[1] + 1 };
					node.Neighbors[node.NumNeighbors] = CoordToIndex(neighborCoord);
					node.NumNeighbors++;
				}
			}
		}

		size_t Dijkstras(size_t startIndex, size_t endIndex)
		{
			// Add the start node cost to the priority queue
			m_Visited.resize(g_CavernSize, false);
			m_Costs.resize(g_CavernSize, MAX_SIZE_T);
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
					size_t NewCost = m_Costs[Index] + g_Risk[NeighborIndex];
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
			size_t StartIndex = CoordToIndex(g_StartCoord);
			size_t EndIndex = CoordToIndex(g_EndCoord);

			size_t Cost = Dijkstras(StartIndex, EndIndex);

			std::cout << "Min cost=" << Cost << std::endl;
		}
	};
}

int main()
{
	Day15::Chiton puzzle;
	puzzle.Execute();

	return 0;
}
