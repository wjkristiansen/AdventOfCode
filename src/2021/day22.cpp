#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <list>
#include <assert.h>

#include "CanvasMath.hpp"
#include "day22.h"

namespace Day22
{
    using Vector = Canvas::Math::TVector<int, 3>;
    using Range = std::pair<int, int>;

    static constexpr bool RangesOverlap(const Range& Range1, const Range& Range2)
    {
        return Range1.second > Range2.first && Range2.second > Range1.first;
    }

    struct Box
    {
        Range RangeX;
        Range RangeY;
        Range RangeZ;

        Box(const Range& RangeX, const Range& RangeY, const Range& RangeZ) :
            RangeX(RangeX),
            RangeY(RangeY),
            RangeZ(RangeZ) {}

        constexpr bool OverlapsWith(const Box& o) const
        {
            return RangesOverlap(RangeX, o.RangeX) && RangesOverlap(RangeY, o.RangeY) && RangesOverlap(RangeZ, o.RangeZ);
        }

        constexpr bool IsDegenerate() const
        {
            return (RangeX.first >= RangeX.second) || (RangeY.first >= RangeY.second) || (RangeZ.first >= RangeZ.second);
        }

        constexpr size_t Volume() const
        {
            return size_t(RangeX.second - RangeX.first) * size_t(RangeY.second - RangeY.first) * size_t(RangeZ.second - RangeZ.first);
        }

        // Splits Box such int N new boxes with box CarveOut removed
        std::vector<Box> CarveOutBox(const Box& CarveOut) const
        {
            std::vector<Box> result;
            Box temp = *this;

            if (temp.OverlapsWith(CarveOut) && !CarveOut.IsDegenerate() && !IsDegenerate())
            {
                // Does plane x=CarveOut.RangeX.first intersect?
                if (CarveOut.RangeX.first > temp.RangeX.first && CarveOut.RangeX.first < temp.RangeX.second)
                {
                    // Carve out the Box slice created by the intersecting plane
                    result.emplace_back(Range(temp.RangeX.first, CarveOut.RangeX.first), temp.RangeY, temp.RangeZ);
                    assert(!result.back().IsDegenerate());

                    // Remove the carved-out box from the source box
                    temp.RangeX.first = CarveOut.RangeX.first;
                    assert(!temp.IsDegenerate());
                }

                // Does plane x=CarveOut.RangeX.second intersect?
                if (CarveOut.RangeX.second > temp.RangeX.first && CarveOut.RangeX.second < temp.RangeX.second)
                {
                    // Carve out the Box slice created by the intersecting plane
                    result.emplace_back(Range(CarveOut.RangeX.second, temp.RangeX.second), temp.RangeY, temp.RangeZ);
                    assert(!result.back().IsDegenerate());

                    // Remove the carved-out box from the source box
                    temp.RangeX.second = CarveOut.RangeX.second;
                    assert(!temp.IsDegenerate());
                }

                // Does plane y=CarveOut.RangeY.first intersect?
                if (CarveOut.RangeY.first > temp.RangeY.first && CarveOut.RangeY.first < temp.RangeY.second)
                {
                    // Carve out the Box slice created by the intersecting plane
                    result.emplace_back(temp.RangeX, Range(temp.RangeY.first, CarveOut.RangeY.first), temp.RangeZ);
                    assert(!result.back().IsDegenerate());

                    // Remove the carved-out box from the source box
                    temp.RangeY.first = CarveOut.RangeY.first;
                    assert(!temp.IsDegenerate());
                }

                // Does plane y=CarveOut.RangeY.second intersect?
                if (CarveOut.RangeY.second > temp.RangeY.first && CarveOut.RangeY.second < temp.RangeY.second)
                {
                    // Carve out the Box slice created by the intersecting plane
                    result.emplace_back(temp.RangeX, Range(CarveOut.RangeY.second, temp.RangeY.second), temp.RangeZ);
                    assert(!result.back().IsDegenerate());

                    // Remove the carved-out box from the source box
                    temp.RangeY.second = CarveOut.RangeY.second;
                    assert(!temp.IsDegenerate());
                }

                // Does plane z=CarveOut.RangeY.first intersect?
                if (CarveOut.RangeZ.first > temp.RangeZ.first && CarveOut.RangeZ.first < temp.RangeZ.second)
                {
                    // Carve out the Box slice created by the intersecting plane
                    result.emplace_back(temp.RangeX, temp.RangeY, Range(temp.RangeZ.first, CarveOut.RangeZ.first));
                    assert(!result.back().IsDegenerate());

                    // Remove the carved-out box from the source box
                    temp.RangeZ.first = CarveOut.RangeZ.first;
                    assert(!temp.IsDegenerate());
                }

                // Does plane y=CarveOut.RangeZ.second intersect?
                if (CarveOut.RangeZ.second > temp.RangeZ.first && CarveOut.RangeZ.second < temp.RangeZ.second)
                {
                    // Carve out the Box slice created by the intersecting plane
                    result.emplace_back(temp.RangeX, temp.RangeY, Range(CarveOut.RangeZ.second, temp.RangeZ.second));
                    assert(!result.back().IsDegenerate());

                    // Remove the carved-out box from the source box
                    temp.RangeZ.second = CarveOut.RangeZ.second;
                    assert(!temp.IsDegenerate());
                }
            }

            return result;
        }
    };

    std::ostream& operator<<(std::ostream& os, const Range& r)
    {
        os << "[" << r.first << "," << r.second << "]";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Box& b)
    {
        os << "X" << b.RangeX << ",Y" << b.RangeY << ",Z" << b.RangeZ;
        return os;
    }

    class CCubeSpace
    {
        std::list<Box> m_Boxes;

    public:
        void RemoveBox(const Box& b)
        {
            for (auto it = m_Boxes.begin(); it != m_Boxes.end(); )
            {
                Box temp = *it;
                if (temp.OverlapsWith(b))
                {
                    // Remove the box from the list
                    it = m_Boxes.erase(it);

                    // Insert the new boxes to the beginning of the list
                    std::vector newBoxes = temp.CarveOutBox(b);
                    m_Boxes.insert(m_Boxes.begin() , newBoxes.begin(), newBoxes.end());
                }
                else
                {
                    ++it;
                }
            }
        }
        void AddBox(const Box& b)
        {
            // Remove any box regions intersecting b
            RemoveBox(b);

            // Add b to the end of the list
            m_Boxes.emplace_back(b);
        }

        size_t CountCubes() const
        {
            size_t count = 0;
            for (const Box& b : m_Boxes)
            {
                count += b.Volume();
            }

            return count;
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

                // Ignore empty line
                if (line == "")
                    continue;

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
                    Box box(Range(xmin, xmax + 1), Range(ymin, ymax + 1), Range(zmin, zmax + 1));
                    if (state == true)
                        AddBox(box);
                    else
                        RemoveBox(box);
                }
            }
        }
    };

    void Execute()
    {
        CCubeSpace cubeSpace;
        cubeSpace.ReadFile("day22.txt", 0);
        size_t count = cubeSpace.CountCubes();
        std::cout << "Num cubes: " << count << std::endl;
    }
}