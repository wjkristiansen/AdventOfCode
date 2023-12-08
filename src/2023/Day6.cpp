#include "pch.h"
#include "SolutionFactory.h"

static std::pair<double, double> SolveQuadratic(double a, double b, double c)
{
    double radical = std::sqrt(b * b - 4 * a * c);
    double denom = 2 * a;
    double v1 = (-b - radical) / denom;
    double v2 = (-b + radical) / denom;
    return std::make_pair(v1, v2);
}

void CSolution<6>::Execute(int part)
{
    struct RaceParams
    {
        std::int64_t Duration;
        std::int64_t RecordDistance;
    };

    std::vector<RaceParams> races;

    std::ifstream fstream("2023/Day6.txt");
    std::string line;

    if (part == 1)
    {
        // Get race times...
        std::getline(fstream, line);
        std::istringstream liness(line);
        liness.ignore(32, ':'); // "Time:"
        while (!liness.eof())
        {
            std::int64_t duration;
            liness >> duration;
            races.push_back(RaceParams{ duration, 0 });
        }

        // Get race records...
        std::getline(fstream, line);
        liness = std::istringstream(line);
        liness.ignore(32, ':'); // "Distance:"
        for (int i = 0; i < races.size(); ++i)
        {
            liness >> races[i].RecordDistance;
        }
    }
    else
    {
        // Get race time...
        std::getline(fstream, line);
        std::istringstream liness(line);
        liness.ignore(32, ':'); // "Time:"

        std::int64_t duration = 0;
        while (!liness.eof())
        {
            int c = liness.get();
            if (c >= '0' && c <= '9')
            {
                duration = duration * 10 + (c - '0');
            }
        }

        // Get race record...
        std::getline(fstream, line);
        liness = std::istringstream(line);
        liness.ignore(32, ':'); // "Distance:"

        std::int64_t record = 0;
        while (!liness.eof())
        {
            int c = liness.get();
            if (c >= '0' && c <= '9')
            {
                record = record * 10 + (c - '0');
            }
        }

        races.push_back(RaceParams{ duration, record });
    }

    std::int64_t product = 1;
    for(auto race : races)
    {
        // Using:
        // D -> distance travelled
        // V -> the time the button is held down
        // T -> duration of the race
        // R -> record distance
        //
        // The equation for distance travelled is:
        // D = V(T - V)
        //
        // Written as a polynomial expression:
        // -V^2 +TV - D = 0
        //
        // An inequality can be written to represent the range
        // of possible winning values of V:
        // R < -V^2 + TV
        // or
        // V^2 -TV + R < 0
        //
        // The quadratic formula can be used to find the min and max
        // values of V:
        // Vmin = (T - sqrt(T^2 - 4R)) / 2
        // Vmax = (T + sqrt(T^2 - 4R)) / 2
        double T = double(race.Duration);
        double R = double(race.RecordDistance);
        auto range = SolveQuadratic(1, -T, R);

        // Convert ranges to integers

        double fminT = std::ceil(range.first);
        double fmaxT = std::floor(range.second);

        // Handle inequality
        if (fminT == range.first)
            fminT += 1.0f;
        if (fmaxT == range.second)
            fmaxT -= 1.0f;

        std::int64_t minT((std::int64_t) fminT);
        std::int64_t maxT((std::int64_t) fmaxT);

        std::cout << "Min speed: " << fminT << "(" << minT << "), Max speed: " << fmaxT << "(" << maxT << ")" << std::endl;

        std::int64_t minD = minT * (race.Duration - minT);
        std::int64_t maxD = maxT * (race.Duration - maxT);

        std::cout << "Min distance: " << minD << ", Max distance: " << maxD << std::endl;
        assert(minD > race.RecordDistance);
        assert(maxD > race.RecordDistance);

        std::int64_t count = maxT - minT + 1;

        std::cout << "Count: " << count << std::endl;
        product *= count;

        // Assert that the calculated limits are correct
        minT--;
        maxT++;
        minD = minT * (race.Duration - minT);
        maxD = maxT * (race.Duration - maxT);
        assert(minD <= race.RecordDistance);
        assert(maxD <= race.RecordDistance);
    }

    std::cout << "Product: " << product << std::endl;
}
