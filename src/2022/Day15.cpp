#include "pch.h"
#include "SolutionFactory.h"

struct Point
{
    Point() = default;
    Point(int x, int y) :
        X(x),
        Y(y) {}

    int X = 0;
    int Y = 0;

    bool operator==(const Point& o) const
    {
        return X == o.X && Y == o.Y;
    }
};

namespace std
{
    template<>
    struct hash<Point>
    {
        size_t operator()(const Point& p) const
        {
            return p.X ^ (p.Y << 16);
        }
    };
}

class CSolution15: public CSolutionBase
{
    static int ManhattanDist(const Point& p0, const Point& p1)
    {
        return std::abs(p0.X - p1.X) + std::abs(p0.Y - p1.Y);
    }

    struct Sensor
    {
        Sensor() = delete;
        Sensor(const Point &loc, const Point &beaconLoc) :
            Loc(loc),
            BeaconLoc(beaconLoc),
            DistToBeacon(ManhattanDist(loc, beaconLoc))
        {
        }
        Point Loc;
        Point BeaconLoc;
        int DistToBeacon;
    };

    struct BeaconMap
    {
        std::vector<Sensor> Sensors;
        std::unordered_set<Point> Beacons;
        int MaxDist = 0;
        int XMin = INT_MAX;
        int XMax = INT_MIN;
        int YMin = INT_MAX;
        int YMax = INT_MIN;

        void UpdateFromString(const std::string &s)
        {
            // string contains: 'Sensor at x=<int>, y=<int>: closest beacon is at x=<int>, y=<int>
            std::istringstream ss(s);
            ss.ignore(s.length(), '=');
            Point SensorLoc;
            ss >> SensorLoc.X;
            ss.ignore(s.length(), '=');
            ss >> SensorLoc.Y;

            if (SensorLoc.X < XMin)
                XMin = SensorLoc.X;
            if (SensorLoc.Y < YMin)
                YMin = SensorLoc.Y;
            if (SensorLoc.X > XMax)
                XMax = SensorLoc.X;
            if (SensorLoc.Y > YMax)
                YMax = SensorLoc.Y;

            Point BeaconLoc;
            ss.ignore(s.length(), '=');
            ss >> BeaconLoc.X;
            ss.ignore(s.length(), '=');
            ss >> BeaconLoc.Y;

            if (BeaconLoc.X < XMin)
                XMin = BeaconLoc.X;
            if (BeaconLoc.Y < YMin)
                YMin = BeaconLoc.Y;
            if (BeaconLoc.X > XMax)
                XMax = BeaconLoc.X;
            if (BeaconLoc.Y > YMax)
                YMax = BeaconLoc.Y;

            Sensors.emplace_back(SensorLoc, BeaconLoc);
            Beacons.emplace(BeaconLoc);
            auto Dist = Sensors.back().DistToBeacon;
            if (Dist > MaxDist)
                MaxDist = Dist;
        }
    };

    void Execute(int Part)
    {
        BeaconMap Map;
        std::ifstream fstream("2022/Day15.txt"); 
        for(;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line); 
            Map.UpdateFromString(line);
        }

        std::cout << "Sensor Count: " << Map.Sensors.size() << std::endl;
        std::cout << "Beacon Count: " << Map.Beacons.size() << std::endl;
        std::cout << "Max Dist: " << Map.MaxDist << std::endl;
        std::cout << "X Range: " << Map.XMin << ", " << Map.XMax << std::endl;
        std::cout << "Y Range: " << Map.YMin << ", " << Map.YMax << std::endl;

        auto IsBeacon = [&Map](const Point& p) -> bool
        {
            return Map.Beacons.find(p) != Map.Beacons.end();
        };

        auto NextMaybeBeaconX = [&Map](const Point& p) -> int
        {
            // For each sensor, see if there are any closer to `p` than
            // to its locked beacon
            for (auto &s : Map.Sensors)
            {
                auto Dist = ManhattanDist(p, s.Loc);
                if (Dist <= s.DistToBeacon)
                {
                    // Sensor is already locked onto a beacon a the same distance or less.
                    // Must not be a beacon at point `p`
                    int vdist = abs(p.Y - s.Loc.Y);
                    int hdist = s.DistToBeacon - vdist;
                    return 1 + s.Loc.X + hdist;
                }
            }

            // May a beacon.
            // All sensors are locked onto a beacon less distant than `p`
            return 0;
        };

        if (Part == 1)
        {
            int NotBeaconCount = 0;

            const int TestRow = 2000000;

            for (int X = Map.XMin - Map.MaxDist; X <= Map.XMax + Map.MaxDist; ++X)
            {
                Point p(X, TestRow);
                if (!IsBeacon(p) && (NextMaybeBeaconX(p) > 0))
                {
                    NotBeaconCount++;
                }
            }

            std::cout << "Number of known-non-beacon positions in row " << TestRow << ": " << NotBeaconCount << std::endl;
        }
        else
        {
            int64_t TuningFreq = 0;
            int SearchMinX = 0;
            int SearchMinY = 0;
            int SearchMaxX = 4000000;
            int SearchMaxY = 4000000;

            for (int y = SearchMinY; y <= SearchMaxY; ++y)
            {
                for (int x = SearchMinX; x <= SearchMaxX; )
                {
                    Point p(x, y);
                    int nextX = x + 1;
                    if (!IsBeacon(p))
                    {
                        nextX = NextMaybeBeaconX(p);

                        if (nextX == 0)
                        {
                            TuningFreq = int64_t(p.X) * 4000000 + p.Y;
                            std::cout << "Beacon location: x=" << p.X << ", y=" << p.Y << std::endl;
                            std::cout << "Beacon tuning frequency: " << TuningFreq << std::endl;
                            return;
                        }
                    }
                    x = nextX;
                }
            }
        }
    }
};

DECLARE_SOLUTION(CSolution15, 15, "Beacon Exclusion Zone");
