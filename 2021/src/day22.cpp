#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <assert.h>

#include "CanvasMath.hpp"
#include "day22.h"

namespace Day22
{
    using Vector=Canvas::Math::TVector<int, 3>;

    struct OctNode
    {
        bool State = false; // Only used if ChildNodes.size() == 0
        std::vector<OctNode> ChildNodes;

        OctNode(bool State) :
            State(State) {}
    };

    // Octtree class. Nodes contain 0 or 8 child nodes indexed by relative octant coordinates:
    // 
    // 0 => +X, +Y, +Z
    // 1 => -X, +Y, +Z
    // 2 => -X, -Y, +Z
    // 3 => +X, -Y, +Z
    // 4 => +X, +Y, -Z
    // 5 => -X, +Y, -Z
    // 6 => -X, -Y, -Z
    // 7 => +X, -Y, -Z
    class COctSpace
    {
        OctNode m_Root;
        int m_MaxDepth; // Max dimension is 2 ^ m_MaxDepth
        Vector m_RootMin;

        int DimensionMax() const { return 1UL << m_MaxDepth; }
        int RangeMin() const { return -(DimensionMax() >> 1); }
        int RangeMax() const { return RangeMin() + DimensionMax(); }

        void SplitNode(OctNode& Node)
        {
            if (Node.ChildNodes.size() == 0)
            {
                // Create 8 child nodes with the same state as given Node
                Node.ChildNodes.resize(8, Node.State);
            }
        }

        void MergeNode(OctNode& Node, bool State)
        {
            Node.ChildNodes.clear();
            Node.State = State;
        }

        // Returns true if the result is a leaf node
        bool RecurseSetCuboidState(OctNode& Node, const Vector& NodeMin, int NodeSize, const Vector& CuboidMin, const Vector& CuboidMax, bool State)
        {
            const Vector nodeMax = NodeMin + Vector(NodeSize - 1, NodeSize - 1, NodeSize - 1);

            // Does the node intersect the cuboid?
            if (nodeMax[0] < CuboidMin[0] ||
                nodeMax[1] < CuboidMin[1] ||
                nodeMax[2] < CuboidMin[2] ||
                NodeMin[0] > CuboidMax[0] ||
                NodeMin[1] > CuboidMax[1] ||
                NodeMin[2] > CuboidMax[2])
            {
                // Node does not intersect so just return
                return Node.ChildNodes.size() == 0;
            }

            // Is the node fully contained by the cuboid?
            if (NodeMin[0] >= CuboidMin[0] &&
                NodeMin[1] >= CuboidMin[1] &&
                NodeMin[2] >= CuboidMin[2] &&
                nodeMax[0] <= CuboidMax[0] &&
                nodeMax[1] <= CuboidMax[1] &&
                nodeMax[2] <= CuboidMax[2])
            {
                // Node range is fully contained by cuboid range.
                // Set the node state to 'State' and clear child
                // nodes.
                MergeNode(Node, State);
                return true;
            }

            assert(NodeSize > 1);

            // Recurse down to each of the child nodes, splitting if necessary
            SplitNode(Node);
            int childSize = NodeSize >> 1;
            bool allLeafNodes = true;
            for (int id = 0; id < 8; ++id)
            {
                Vector childMin;

                switch (id)
                {
                case 0:
                    childMin = NodeMin + Vector(childSize, childSize, childSize);
                    break;
                case 1:
                    childMin = NodeMin + Vector(0, childSize, childSize);
                    break;
                case 2:
                    childMin = NodeMin + Vector(0, 0, childSize);
                    break;
                case 3:
                    childMin = NodeMin + Vector(childSize, 0, childSize);
                    break;
                case 4:
                    childMin = NodeMin + Vector(childSize, childSize, 0);
                    break;
                case 5:
                    childMin = NodeMin + Vector(0, childSize, 0);
                    break;
                case 6:
                    childMin = NodeMin + Vector(0, 0, 0);
                    break;
                case 7:
                    childMin = NodeMin + Vector(childSize, 0, 0);
                    break;
                default:
                    throw std::exception();
                    break;
                }

                bool IsLeafNode = RecurseSetCuboidState(Node.ChildNodes[id], childMin, childSize, CuboidMin, CuboidMax, State);
                allLeafNodes = allLeafNodes && IsLeafNode;
            }

            if (allLeafNodes)
            {
                int id = 0;
                for (; id < 8; ++id)
                {
                    if (Node.ChildNodes[id].State != State)
                        break;
                }

                if (id == 8)
                {
                    MergeNode(Node, State);
                    return true;
                }
            }

            return false;
        }

        size_t RecurseCountImpl(const OctNode& Node, int NodeSize, bool State)
        {
            size_t Count = 0;

            if (Node.ChildNodes.size() > 0)
            {
                assert(NodeSize > 1);
                int childSize = NodeSize >> 1;
                for (auto& Child : Node.ChildNodes)
                {
                    Count += RecurseCountImpl(Child, childSize, State);
                }
            }
            else
            {
                if (Node.State == State)
                {
                    Count = NodeSize * NodeSize * NodeSize;
                }
            }

            return Count;
        }

    public:
        COctSpace(int MaxDepth) :
            m_MaxDepth(MaxDepth),
            m_Root(false)
        {
            assert(MaxDepth < 32);
            m_RootMin = Vector(RangeMin(), RangeMin(), RangeMin());
        }

        void ReadFile(const std::string& fileName, int limit)
        {
            std::ifstream fstream(fileName);

            for (; !fstream.eof();)
            {
                size_t offset = 0;
                std::string line;
                std::getline(fstream, line);
                if (fstream.eof())
                    return;

                bool state = false;
                if (line.rfind("on ", offset) == offset)
                {
                    state = true;
                    offset += 3;
                }
                else if (line.rfind("off ", offset) == offset)
                {
                    state = false;
                    offset += 4;
                }
                else
                {
                    throw std::exception();
                }

                if (line.rfind("x=", offset) != offset)
                    throw std::exception();

                offset += 2;

                size_t end;

                end = line.find_first_of("..", offset);
                if (end == line.npos)
                    throw std::exception();
                int xmin = std::stoi(line.substr(offset, end));
                offset = end + 2;
                
                end = line.find_first_of(",y=", offset);
                if (end == line.npos)
                    throw std::exception();
                int xmax = std::stoi(line.substr(offset, end));
                offset = end + 3;

                end = line.find_first_of("..", offset);
                if (end == line.npos)
                    throw std::exception();
                int ymin = std::stoi(line.substr(offset, end));
                offset = end + 2;

                end = line.find_first_of(",z=", offset);
                if (end == line.npos)
                    throw std::exception();
                int ymax = std::stoi(line.substr(offset, end));
                offset = end + 3;

                end = line.find_first_of("..", offset);
                if (end == line.npos)
                    throw std::exception();
                int zmin = std::stoi(line.substr(offset, end));
                offset = end + 2;

                int zmax = std::stoi(line.substr(offset));
                offset = end + 1;

                if (limit == 0 || 
                    (xmin >= -limit && xmax <= limit &&
                    ymin >= -limit && ymax <= limit &&
                    zmin >= -limit && zmax <= limit))
                {
                    SetCuboidState(Vector(xmin, ymin, zmin), Vector(xmax, ymax, zmax), state);
                }
            }
        }

        void SetCuboidState(const Vector& Min, const Vector& Max, bool State)
        {
            int rmin = RangeMin();
            int rmax = RangeMax();
            // Find or create all oct-tree nodes fully contained by range [Min, Max]
            if (Max[0] > rmax || Max[1] > RangeMax() || Max[2] > RangeMax() ||
                Min[0] < rmin || Min[1] < RangeMin() || Min[2] < RangeMin())
                throw std::exception();
            RecurseSetCuboidState(m_Root, m_RootMin, DimensionMax(), Min, Max, State);
        }

        size_t CountNodes(bool State)
        {
            return RecurseCountImpl(m_Root, DimensionMax(), State);
        }
    };

    void Execute()
    {
        COctSpace OctSpace(20);
        OctSpace.ReadFile("day22.txt", 50);

        //OctSpace.SetCuboidState(Vector(10, 10, 10), Vector(12, 12, 12), true);
        //OctSpace.SetCuboidState(Vector(11, 11, 11), Vector(13, 13, 13), true);
        //OctSpace.SetCuboidState(Vector(9, 9, 9), Vector(11, 11, 11), false);
        //OctSpace.SetCuboidState(Vector(10, 10, 10), Vector(10, 10, 10), true);

        size_t onCount = OctSpace.CountNodes(true);
        std::cout << "On count: " << onCount << std::endl;
    }
}