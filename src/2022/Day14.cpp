#include "../pch.h"
#include "../SolutionFactory.h"

struct Point
{
    Point(int x, int y) :
        X(x),
        Y(y) {}
    int X;
    int Y;
};

struct Path
{
    int MinX = std::numeric_limits<int>::max();
    int MaxX = 0;
    int MaxY = 0;
    std::vector<Point> Points;

    void FromString(const std::string &s)
    {
        std::istringstream ss(s);

        for(;!ss.eof();)
        {
            char c;
            std::string arrow;
            int x, y;
            ss >> x;
            ss >> c;
            assert(c == ',');
            ss >> y;

            Points.emplace_back(x, y);

            if (x > MaxX)
                MaxX = x;

            if (x < MinX)
                MinX = x;

            if (y > MaxY)
                MaxY = y;

            ss >> arrow;
            if(ss.eof())
                break; // Last entry
            assert(arrow == "->");
        }
    }
};

struct SandMap
{
    int MinX;
    int Width;
    int Height;
    std::vector<char> FlatMap;

    SandMap(int minX, int maxX, int maxY) :
        MinX(minX),
        Width(1 + maxX - minX),
        Height(maxY + 1),
        FlatMap(Width * Height, '.') {}

    char GetValue(const Point &p) const
    {
        if (p.Y == Height - 1)
            return '#';

        if (p.X < MinX || p.X >= MinX + Width)
            return '.';

        int x = p.X - MinX;
        size_t Index = p.Y * Width + (x % Width);
        return FlatMap[Index];
    }

    void SetValue(const Point &p, char value)
    {
        int x = p.X - MinX;
        size_t Index = p.Y * Width + (x % Width);
        FlatMap[Index] = value;
    }

    void SetValues(const Path& path, char c)
    {
        Point Start = path.Points[0];
        for (size_t i = 1; i < path.Points.size(); ++i)
        {
            Point End = path.Points[i];

            if (Start.Y == End.Y)
            {
                // Horizontal
                int y = Start.Y;
                int Dir = Start.X < End.X ? 1 : -1;
                for (int x = Start.X; x != End.X; x += Dir)
                {
                    SetValue(Point(x, y), c);
                }
            }
            else
            {
                // Vertical
                int x = Start.X;
                int Dir = Start.Y < End.Y ? 1 : -1;
                for (int y = Start.Y; y != End.Y; y += Dir)
                {
                    SetValue(Point(x, y), c);
                }
            }
            Start = End;
        }

        // Set the last value
        SetValue(path.Points.back(), c);
    }

    void Print() const
    {
        std::cout << "X min: " << MinX << std::endl;
        std::cout << "X max: " << MinX + Width - 1 << std::endl;
        for (int y = 0; y < Height; ++y)
        {
            std::cout << std::setw(4) << y << " ";

            for (int x = 0; x < Width; ++x)
            {
                std::cout << GetValue(Point(x + MinX, y));
            }

            std::cout << std::endl;
        }
    }
};

void CSolution<2022, 14>::Execute()
{
    int MinX = std::numeric_limits<int>::max();
    int MaxX = 0;
    int MaxY = 0;
    std::vector<Path> Paths;
    std::ifstream fstream("Day14.txt"); 
    for(;!fstream.eof();)
    {
        std::string line;
        std::getline(fstream, line);
        if(line.empty())
            break;
        Paths.emplace_back();
        auto &p = Paths.back();
        p.FromString(line);

        if (p.MinX < MinX)
            MinX = p.MinX;
        if (p.MaxX > MaxX)
            MaxX = p.MaxX;
        if (p.MaxY > MaxY)
            MaxY = p.MaxY;
    }

    MaxY += 2;
    MinX -= MaxY;
    MaxX += MaxY;

    SandMap Map(MinX, MaxX, MaxY);

    for (auto& p : Paths)
    {
        Map.SetValues(p, '#');
    }

    const Point SandStart(500, 0);

    Map.Print();

    // Run simulation

    auto SimTick = [&Map](Point &Sand) -> bool
    {
        assert(Sand.Y == 0 || Map.GetValue(Sand) == '.');

        Point SandNext(Sand.X, Sand.Y + 1);
        if (SandNext.Y == Map.Height)
        {
            Sand = SandNext;
            return false; // Finished
        }

        if (Map.GetValue(SandNext) != '.')
        {
            SandNext.X--;
            if (Map.GetValue(SandNext) != '.')
            {
                SandNext.X += 2;
                if (Map.GetValue(SandNext) != '.')
                {
                    Map.SetValue(Sand, 'O');
                    return false;
                }
            }
        }

        Sand = SandNext;

        return true;
    };

    int Units = 0;
    for (;;)
    {
        Point Sand(SandStart);
        for (; SimTick(Sand);) {}
        ++Units;
        if (Sand.Y == 0)
            break;
    }

    Map.Print();

    std::cout << "Number of sand units: " << Units << std::endl;
}