#include "pch.h"
#include "SolutionFactory.h"

class CSolution14 : public CSolutionBase
{
public:
    CSolution14() : CSolutionBase()
    {}

    struct Bot
    {
        Bot(int posX, int posY, int velX, int velY) :
            Position(posX, posY),
            Velocity(velX, velY) {}
        
        std::pair<int, int> Position;
        std::pair<int, int> Velocity;
    };

    const int GridWidth = 101;
    const int GridHeight = 103;
    // const int GridWidth = 11;
    // const int GridHeight = 7;

    std::vector<Bot> m_Bots;

    bool ReadBot(std::ifstream& fstream)
    {
        if (fstream.eof())
            return false;
        
        std::string line;
        std::getline(fstream, line);
        if (line.empty())
            return false;

        std::string pattern = R"(p=([0-9]+),([0-9]+) v=([-]*[0-9]+),([-]*[0-9]+))";
        std::regex regex(pattern, std::regex::ECMAScript);
        std::smatch matches;
        if (!std::regex_search(line, matches, regex))
            return false;

        m_Bots.emplace_back(std::stoi(matches[1]), std::stoi(matches[2]), std::stoi(matches[3]), std::stoi(matches[4]));

        return true;
    }

    void MoveBots(int seconds)
    {
        for (auto &bot : m_Bots)
        {
            bot.Position.first = (bot.Position.first + bot.Velocity.first * seconds) % GridWidth;
            if (bot.Position.first < 0)
                bot.Position.first += GridWidth;
            bot.Position.second = (bot.Position.second + bot.Velocity.second * seconds) % GridHeight;
            if (bot.Position.second < 0)
                bot.Position.second += GridHeight;
        }
    }

    void PrintBots()
    {
        for (auto& bot : m_Bots)
        {
            std::cout << "Pos: " << bot.Position.first << "," << bot.Position.second << " ";
            std::cout << "Vel: " << bot.Velocity.first << "," << bot.Velocity.second << std::endl;
        }
    }

    void PrintGrid()
    {
        AsciiGrid grid(GridWidth, GridHeight, '.');

        for (auto& bot : m_Bots)
        {
             if (grid.GetValue(bot.Position.second, bot.Position.first) == '.')
                grid.SetValue('1', bot.Position.second, bot.Position.first);
            else
                grid.Data()[bot.Position.second][bot.Position.first]++;
        }

        grid.Print();
    }

    int QuadrantIndex(const std::pair<int, int>& coord)
    {
        if (coord.first == GridWidth / 2 || coord.second == GridHeight / 2)
            return -1;
        
        if (coord.first < GridWidth / 2)
        {
            if (coord.second < GridHeight / 2)
                return 0;
            else
                return 2;
        }

        if (coord.second < GridHeight / 2)
            return 1;

        return 3;
    }

    int SafetyFactor()
    {
        int quadrantCounts[4] = {};

        for (auto& bot : m_Bots)
        {
            int index = QuadrantIndex(bot.Position);
            if (index < 0)
                continue;
            
            quadrantCounts[index]++;
        }

        return quadrantCounts[0] * quadrantCounts[1] * quadrantCounts[2] * quadrantCounts[3];
    }

    virtual void Execute(int part)
    {
        std::ifstream fstream("2024/Day14.txt");

        while (!fstream.eof())
        {
            if (!ReadBot(fstream))
                break;
        }

        PrintBots();

        MoveBots(100);

        PrintGrid();

        std::cout << "Safety Factor: " << SafetyFactor() << std::endl;
    }
};

DECLARE_SOLUTION(CSolution14, 14, "Restroom Redoubt");
