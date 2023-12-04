#include "pch.h"
#include "SolutionFactory.h"

void CSolution<4>::Execute(int part)
{
    std::ifstream fstream("2023/Day4.txt");
    std::string line;
    int powerSum = 0;

    struct Card
    {
        std::set<int> WinningNumbers;
        std::vector<int> PlayerNumbers;

        int Power() const
        {
            int power = 0;
            for( int w : PlayerNumbers)
            {
                if(WinningNumbers.find(w) != WinningNumbers.end())
                {
                    if( power == 0)
                        power = 1;
                    else
                        power = power << 1;
                }
            }
            return power;
        }
    };

    for(;!fstream.eof();)
    {
        std::getline(fstream, line);
        if(line.empty())
            break;

        std::istringstream iss(line);
        int cardId = 0;
        iss.ignore(5, ' '); // "Card "
        iss >> cardId;
        iss.ignore(2, ' '); // ": "

        Card card;

        // Read winning numbers
        std::string numbers;
        std::getline(iss, numbers, '|');
        std::istringstream wss(numbers);
        for (; !wss.eof();)
        {
            int number = 0;
            wss >> number;
            card.WinningNumbers.insert(number);
        }

        // Read player numbers
        std::getline(iss, numbers);
        std::istringstream pss(numbers);
        for (; !pss.eof();)
        {
            int number = 0;
            pss >> number;
            card.PlayerNumbers.push_back(number);
        }
        
        int power = card.Power();
        powerSum += power;
        std::cout << "Card " << cardId << ": " << power << std::endl;
    }

    std::cout << "Sum of card powers: " << powerSum << std::endl;
}