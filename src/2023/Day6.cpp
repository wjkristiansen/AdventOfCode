#include "pch.h"
#include "SolutionFactory.h"

static std::pair<float, float> SolveQuadratic(float a, float b, float c)
{
    float radical = std::sqrt(b * b - 4 * a * c);
    float denom = 2 * a;
    float v1 = (-b - radical) / denom;
    float v2 = (-b + radical) / denom;
    return std::make_pair(v1, v2);
}

void CSolution<6>::Execute(int part)
{
    struct RaceParams
    {
        int Duration;
        int RecordDistance;
    };

    std::vector<RaceParams> races;

    std::ifstream fstream("2023/Day6.txt");
    std::string line;

    // Get race times...
    std::getline(fstream, line);
    std::istringstream liness (line);
    liness.ignore(32, ':'); // "Time:"
    while(!liness.eof())
    {
        int duration;
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

    int product = 1;
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
        float T = float(race.Duration);
        float R = float(race.RecordDistance);
        auto range = SolveQuadratic(1, -T, R);

        // Convert ranges to integers

        float fminT = std::ceil(range.first);
        float fmaxT = std::floor(range.second);

        // Handle inequality
        if (fminT == range.first)
            fminT += 1.0f;
        if (fmaxT == range.second)
            fmaxT -= 1.0f;

        int minT((int) fminT);
        int maxT((int) fmaxT);

        std::cout << "Min speed: " << fminT << "(" << minT << "), Max speed: " << fmaxT << "(" << maxT << ")" << std::endl;

        int minD = minT * (race.Duration - minT);
        int maxD = maxT * (race.Duration - maxT);

        std::cout << "Min distance: " << minD << ", Max distance: " << maxD << std::endl;
        assert(minD > race.RecordDistance);
        assert(maxD > race.RecordDistance);

        int count = maxT - minT + 1;

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
