#include "pch.h"
#include "SolutionFactory.h"

class CSolution13 : public CSolutionBase
{
public:
    CSolution13() : CSolutionBase()
    {}

    // Equation of the form AX + BY = C
    struct Equation
    {
        int64_t A;
        int64_t B;
        int64_t C;

        Equation() = default;
        Equation(int64_t a, int64_t b, int64_t c) :
            A(a),
            B(b),
            C(c) {}

        Equation(const Equation& o) = default;
        Equation &operator=(const Equation& o) = default;

        Equation operator+(const Equation& o) const
        {
            return Equation(A + o.A, B + o.B, C + o.C);
        }

        Equation operator-(const Equation& o) const
        {
            return Equation(A - o.A, B - o.B, C - o.C);
        }

        Equation operator*(int64_t v) const
        {
            return Equation(A * v, B * v, C * v);
        }

        Equation Scale(int64_t v) const
        {
            return Equation(A * v, B * v, C * v);
        }
    };

    static bool SolveSimEquation(const Equation& eq1, const Equation& eq2, std::pair<int64_t, int64_t> &result)
    {
        Equation eq1prime = eq1 * eq2.A;
        Equation eq2prime = eq2 * eq1.A;

        Equation eq3 = eq1prime - eq2prime;
        if (eq3.B == 0 || eq1.A == 0)
            return false; // Unsolvable due to divide by zero
        
        size_t y = eq3.C / eq3.B;
        if (y * eq3.B != eq3.C)
            return false; // Inexact result

        int64_t cPrime = eq1.C - eq1.B * y;
        int64_t x = cPrime / eq1.A;
        if (x * eq1.A != cPrime)
            return false; // Inexact result

        if (x < 0 || y < 0)
            return false; // Ignore negative results
        
        result = std::pair(x, y);
        return true;
    }

    Equation m_Eq1;
    Equation m_Eq2;

    bool ReadEquation(std::ifstream& fstream)
    {
        if (fstream.eof())
            return false;
        
        std::string line;
        std::getline(fstream, line);
        if (line.empty())
            return false;

        // Button A
        const std::string buttonPattern = R"(Button ([A-Z]): X\+([0-9]+), Y\+([0-9]+))";
        const std::regex buttonRegex(buttonPattern, std::regex_constants::ECMAScript);

        std::smatch buttonMatchesA;
        if (!std::regex_search(line, buttonMatchesA, buttonRegex))
            return false;

        m_Eq1.A = std::stoll(buttonMatchesA[2]);
        m_Eq2.A = std::stoll(buttonMatchesA[3]);

        // Button B
        std::getline(fstream, line);
        if (line.empty())
            return false;

        std::smatch buttonMatchesB;
        if (!std::regex_search(line, buttonMatchesB, buttonRegex))
            return false;

        m_Eq1.B = std::stoll(buttonMatchesB[2]);
        m_Eq2.B = std::stoll(buttonMatchesB[3]);

        // Prize
        const std::string prizePattern = R"(Prize: X=([0-9]+), Y=([0-9]+))";
        const std::regex prizeRegex(prizePattern, std::regex_constants::ECMAScript);
        
        std::getline(fstream, line);
        if (line.empty())
            return false;

        std::smatch prizeMatches;
        if (!std::regex_search(line, prizeMatches, prizeRegex))
            return false;

        m_Eq1.C = std::stoll(prizeMatches[1]);
        m_Eq2.C = std::stoll(prizeMatches[2]);

        return true;
    }

    virtual void Execute(int part)
    {
        std::ifstream fstream("2024/Day13.txt");
        int64_t cost = 0;
        while (!fstream.eof())
        {
            if (!ReadEquation(fstream))
                continue;

            std::pair<int64_t, int64_t> result;

            if (part == 2)
            {
                m_Eq1.C += 10000000000000LL;
                m_Eq2.C += 10000000000000LL;
            }

            if (SolveSimEquation(m_Eq1, m_Eq2, result))
            {
                cost += 3 * result.first + result.second;
            }
        }
        
        std::cout << "Total Cost: " << cost << std::endl;
    }
};

DECLARE_SOLUTION(CSolution13, 13, "Claw Contraption");
