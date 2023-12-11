#include "pch.h"
#include "SolutionFactory.h"

class CSolution7 : public CSolutionBase
{
public:
    CSolution7() : CSolutionBase()
    {
    }

    class Hand
    {
    public:
        enum TYPE
        {
            UNKNOWN,
            HIGH_CARD,
            ONE_PAIR,
            TWO_PAIR,
            THREE_OF_A_KIND,
            FULL_HOUSE,
            FOUR_OF_A_KIND,
            FIVE_OF_A_KIND
        };

        int Bid;
        std::string Cards;
        int MaxDuplicates = 0;
        int NumJokers = 0;
        TYPE Type;

        Hand(const std::string& cards, int bid) :
            Bid(bid),
            Cards{ cards }
        {
            std::map<char, int> CardMap;
            for (char c : cards)
            {
                if (c == 'J')
                {
                    NumJokers++;
                }
                else
                {
                    if (CardValue(c) > 0)
                    {
                        CardMap[c] += 1;
                        if (CardMap[c] > MaxDuplicates)
                            MaxDuplicates = CardMap[c];
                    }
                }
            }

            // Calculate type of hand
            Type = UNKNOWN;

            switch (CardMap.size())
            {
            case 0:
            case 1:
                assert(CardMap.size() == 1 || NumJokers == 5);
                Type = FIVE_OF_A_KIND;
                break;
            case 2:
                if (MaxDuplicates + NumJokers == 4)
                    Type = FOUR_OF_A_KIND;
                else
                    Type = FULL_HOUSE;
                break;
            case 3:
                if (MaxDuplicates + NumJokers == 3)
                    Type = THREE_OF_A_KIND;
                else
                    Type = TWO_PAIR;
                break;
            case 4:
                Type = ONE_PAIR;
                break;
            case 5:
                Type = HIGH_CARD;
                break;
            }
        }

        static constexpr int CardValue(char c)
        {
            switch (c)
            {
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'T': return 10;
            case 'J': return 0;
            case 'Q': return 12;
            case 'K': return 13;
            case 'A': return 14;
            }

            return -1;
        }

        bool operator<(const Hand& o) const
        {
            if (Type < o.Type)
                return true;

            if (Type == o.Type)
            {
                for (int i = 0; i < 5; ++i)
                {
                    if (CardValue(Cards[i]) < CardValue(o.Cards[i]))
                        return true;

                    if (Cards[i] != o.Cards[i])
                        return false;
                }
            }

            return false;
        }
    };

    virtual void Execute(int part)
    {
        std::ifstream fstream("2023/Day7.txt");
        std::string line;

        std::set<Hand> hands;

        for (; !fstream.eof();)
        {
            std::getline(fstream, line);
            if (line.empty())
                break;
            std::istringstream ss(line);
            std::string cards;
            int bid;
            ss >> cards >> bid;
            hands.emplace(Hand(cards, bid));
        }

        int rank = 1;
        std::int64_t sum = 0;
        for (auto& hand : hands)
        {
            //        std::cout << "Hand " << rank << ": " << hand.Cards << ", Bid: " << hand.Bid << std::endl;
            sum += hand.Bid * rank;
            ++rank;
        }

        std::cout << "Winnings: " << sum << std::endl;
    }
};

static CSolutionFactory::CSolutionAllocator<CSolution7> SolutionAllocator(7, "Camel Cards");
