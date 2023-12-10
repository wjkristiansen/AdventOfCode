#include "pch.h"
#include "SolutionFactory.h"

class CSolution4 : public CSolutionBase
{
public:
    CSolution4() : CSolutionBase(4, "Scratchcards")
    {
    }

    virtual void Execute(int part)
    {
        std::ifstream fstream("2023/Day4.txt");
        std::string line;
        struct Card
        {
            int Id;
            int MatchCount;
            int Copies;
            std::set<int> WinningNumbers;
            std::vector<int> PlayerNumbers;

        private:
            int CalcMatchCount() const
            {
                int matchCount = 0;
                for (int w : PlayerNumbers)
                {
                    if (WinningNumbers.find(w) != WinningNumbers.end())
                    {
                        matchCount++;
                    }
                }
                return matchCount;
            }

        public:
            Card() = default;
            Card(const Card&) = default;
            Card(Card&& o)
            {
                Id = o.Id;
                MatchCount = o.MatchCount;
                Copies = o.Copies;
                WinningNumbers = std::move(o.WinningNumbers);
                PlayerNumbers = std::move(o.PlayerNumbers);
                o.Id = 0;
                o.MatchCount = 0;
                o.Copies = 0;
            }

            int Read(std::istream& iss)
            {
                int cardId = 0;
                iss.ignore(5, ' '); // "Card "
                iss >> cardId;
                iss.ignore(2, ' '); // ": "

                // Read winning numbers
                std::string numbers;
                std::getline(iss, numbers, '|');
                std::istringstream wss(numbers);
                for (; !wss.eof();)
                {
                    int number = 0;
                    wss >> number;
                    WinningNumbers.insert(number);
                }

                // Read player numbers
                std::getline(iss, numbers);
                std::istringstream pss(numbers);
                for (; !pss.eof();)
                {
                    int number = 0;
                    pss >> number;
                    PlayerNumbers.push_back(number);
                }

                MatchCount = CalcMatchCount();
                Id = cardId;
                Copies = 1;
                return cardId;
            }

            int Power() const
            {
                int matchCount = MatchCount;
                return matchCount == 0 ? 0 : 1 << (matchCount - 1);
            }
        };

        if (part == 1)
        {
            int powerSum = 0;

            for (; !fstream.eof();)
            {
                std::getline(fstream, line);
                if (line.empty())
                    break;

                std::istringstream iss(line);

                Card card;
                int cardId = card.Read(iss);

                int power = card.Power();
                powerSum += power;
                std::cout << "Card " << cardId << ": " << power << std::endl;
            }

            std::cout << "Sum of card powers: " << powerSum << std::endl;
        }
        else
        {
            std::map<int, Card> Cards;
            for (; !fstream.eof();)
            {
                std::getline(fstream, line);
                if (line.empty())
                    break;

                std::istringstream iss(line);
                Card card;
                int cardId = card.Read(iss);

                Cards.emplace(cardId, std::move(card));
            }

            // Calculate total card count
            int totalCards = 0;
            for (auto& [_, card] : Cards)
            {
                // Fixup copy counts of subsequent cards
                for (int i = 0; i < card.MatchCount; ++i)
                {
                    auto it = Cards.find(card.Id + i + 1);
                    if (it != Cards.end())
                    {
                        it->second.Copies += card.Copies;
                    }
                }

                std::cout << "Card: Id: " << card.Id << ", Copies: " << card.Copies << std::endl;

                // Add to total
                totalCards += card.Copies;
            }

            std::cout << "Total Cards: " << totalCards << std::endl;
        }
    }
};

static CSolution4 Solution;

