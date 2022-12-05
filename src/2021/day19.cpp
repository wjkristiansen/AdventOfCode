#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <assert.h>
#include "day19.h"

namespace Day19
{
    Vector Transform(const Vector& p, const Matrix& rot, const Vector &trans)
    {
        return (p * rot) + trans;
    }

    struct ScannerData
    {
        Matrix Rot = Matrix::Identity();
        Vector Offset = Vector(0, 0, 0);
        std::vector<Vector> Points;
        std::unordered_map<int, std::vector<Vector>> Transforms;
        const std::vector<Vector>& GetTransfomed(int rotIndex)
        {
            auto it = Transforms.find(rotIndex);
            if (it == Transforms.end())
            {
                it = Transforms.emplace(rotIndex, Points.size()).first;
                for (size_t i = 0; i < Points.size(); ++i)
                {
                    it->second[i] = Transform(Points[i], Rotations[rotIndex], Vector(0, 0, 0));
                }
            }
            return it->second;
        }
    };

    size_t HaveCommonPoints(ScannerData& a, ScannerData& b, Vector &offset, Matrix &rot)
    {
        offset = Vector();
        rot = Matrix();

        for (int i = 0; i < 24; ++i)
        {
            // Rotate all points in B to each of 24 different
            // axis-aligned rotations.
            const std::vector<Vector> &PointsX = b.GetTransfomed(i);

            // Overlap each point with the transformed location of each other point
            for(const Vector& bbase : PointsX)
            {
                for (const Vector& abase : a.Points)
                {
                    Vector Offset = abase - bbase;
                    size_t MatchCount = 0;

                    for (const Vector& bp : PointsX)
                    {
                        for (const Vector& ap : a.Points)
                        {
                            Vector bRel = bp + Offset;
                            if (std::abs(bRel[0]) <= 1000 &&
                                std::abs(bRel[1]) <= 1000 &&
                                std::abs(bRel[2]) <= 1000 &&
                                bRel == ap)
                            {
                                MatchCount++;
                                if (MatchCount == 12)
                                {
                                    offset = Offset;
                                    rot = Rotations[i];
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return 0;
    }

    struct VectorLess
    {
        bool operator()(const Vector& v1, const Vector& v2) const
        {
            return v1[0] == v2[0] ? (v1[1] == v2[1] ? (v1[2] > v2[2]) : v1[1] < v2[1]) : v1[0] < v2[0];
        }
    };

    class CScannerSet
    {
        std::vector<ScannerData> m_ScannerData;
        std::deque<size_t> m_UnmappedScannerIds;
        std::set<Vector, VectorLess> m_BeaconPoints;

    public:
        const ScannerData& GetScannerData(size_t index) const { return m_ScannerData[index]; }

        void ReadScannerData(const std::string& fileName)
        {
            m_ScannerData.clear();
            m_UnmappedScannerIds.clear();

            std::ifstream fstream(fileName);
            std::string line;
            for (; !fstream.eof();)
            {
                std::getline(fstream, line);
                if (!line.empty())
                {
                    size_t offset = line.rfind("--- scanner ", 0);
                    if (offset != std::string::npos)
                    {
                        ScannerData& Data = m_ScannerData.emplace_back();
                        for (; !fstream.eof();)
                        {
                            std::getline(fstream, line);
                            if (line.empty())
                                break;

                            Vector point;
                            std::string valueString;
                            std::istringstream ss(line);
                            std::getline(ss, valueString, ',');
                            point[0] = std::stoi(valueString);
                            std::getline(ss, valueString, ',');
                            point[1] = std::stoi(valueString);
                            std::getline(ss, valueString);
                            point[2] = std::stoi(valueString);
                            Data.Points.push_back(point);
                        }
                    }
                }
            }

            for (size_t i = 1; i < m_ScannerData.size(); ++i)
            {
                m_UnmappedScannerIds.push_back(i);
            }

            // Add all of scanner 0 points to m_BeaconPoints
            for (const Vector& v : m_ScannerData[0].Points)
            {
                m_BeaconPoints.emplace(v);
            }
        }

        void PrintPoint(const Vector& p)
        {
            std::cout << "[" << p[0] << "," << p[1] << "," << p[2] << "]";
        }

        void PrintGetScannerData()
        {
            for (size_t i = 0; i < m_ScannerData.size(); ++i)
            {
                ScannerData& Data = m_ScannerData[i];
                std::cout << "scanner " << i << std::endl;
                for (const Vector& point : Data.Points)
                {
                    PrintPoint(point);
                    std::cout << std::endl;
                }
            }
        }

        int MaxManhattanDistance()
        {
            int MaxManhattan = 0;

            for (int i = 1; i < m_ScannerData.size(); ++i)
            {
                for (int j = i; j < m_ScannerData.size(); ++j)
                {
                    Vector Delta = m_ScannerData[i].Offset - m_ScannerData[j].Offset;
                    Delta = Vector(std::abs(Delta[0]), std::abs(Delta[1]), std::abs(Delta[2]));
                    MaxManhattan = std::max(MaxManhattan, Delta[0] + Delta[1] + Delta[2]);
                }
            }

            return MaxManhattan;
        }

        void PrintBeaconPoints()
        {
            for (const Vector& p : m_BeaconPoints)
            {
                PrintPoint(p);
                std::cout << std::endl;
            }
            std::cout << "Total Points: " << m_BeaconPoints.size() << std::endl;
            std::cout << "Max Manhattan: " << MaxManhattanDistance() << std::endl;
        }

        // Recursively search for unmapped scanner data
        // with common beacon data with the given scanner index.
        void MapScannerData(size_t scannerId, const Matrix &rot, const Vector &offset)
        {
            std::unordered_set<size_t> Checked;

            if (m_UnmappedScannerIds.empty())
                return;

            while(!m_UnmappedScannerIds.empty())
            {
                size_t unmappedId = m_UnmappedScannerIds.front();

                if (Checked.find(unmappedId) != Checked.end())
                    break;

                Checked.emplace(unmappedId);

                // Always move the id to the back of the queue
                m_UnmappedScannerIds.pop_front();
                m_UnmappedScannerIds.push_back(unmappedId);

                Matrix LocalRot = Matrix::Identity();
                Vector LocalOffset(0, 0, 0);
                if (HaveCommonPoints(m_ScannerData[scannerId], m_ScannerData[unmappedId], LocalOffset, LocalRot))
                {
                    m_UnmappedScannerIds.pop_back();
                    std::cout << "Match!!! " << m_UnmappedScannerIds.size() << " remaining" << std::endl;

                    Matrix FullRot = LocalRot * rot;
                    Vector FullOffset = ( LocalOffset * rot) + offset;

                    m_ScannerData[unmappedId].Offset = FullOffset;

                    // Transform all of scannerN points into scanner0 space
                    for (const Vector& p : m_ScannerData[unmappedId].Points)
                    {
                        Vector BeaconPoint = (p * FullRot) + FullOffset;
                        m_BeaconPoints.emplace(BeaconPoint);
                    }

                    MapScannerData(unmappedId, FullRot, FullOffset);
                }
            }
        }
    };

    void Execute()
    {
        CScannerSet ScannerSet;
        ScannerSet.ReadScannerData("day19.txt");

        ScannerSet.MapScannerData(0, Matrix::Identity(), Vector(0, 0, 0));

        ScannerSet.PrintBeaconPoints();
    }
}
