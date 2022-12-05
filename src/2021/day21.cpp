#include <iostream>
#include <vector>
#include <unordered_map>
#include <assert.h>

#include "day21.h"

namespace Day21
{
    // Per turn, there are 27 (3^3) possible roll results a 3-sided 'quantum' die
    static const int RollResults[] =
    {
        1 + 1 + 1,
        1 + 1 + 2,
        1 + 1 + 3,
        1 + 2 + 1,
        1 + 2 + 2,
        1 + 2 + 3,
        1 + 3 + 1,
        1 + 3 + 2,
        1 + 3 + 3,
        2 + 1 + 1,
        2 + 1 + 2,
        2 + 1 + 3,
        2 + 2 + 1,
        2 + 2 + 2,
        2 + 2 + 3,
        2 + 3 + 1,
        2 + 3 + 2,
        2 + 3 + 3,
        3 + 1 + 1,
        3 + 1 + 2, 
        3 + 1 + 3,
        3 + 2 + 1,
        3 + 2 + 2,
        3 + 2 + 3,
        3 + 3 + 1,
        3 + 3 + 2,
        3 + 3 + 3,
    };

    static const int NewUniversesPerTurn = _countof(RollResults);
    static const int WinningScore = 21;
    static const uint8_t Player1Start = 6;
    static const uint8_t Player2Start = 9;

    class Game
    {
        std::unordered_map<uint32_t, uint8_t> m_EndPosTable;
        std::unordered_map<uint32_t, std::pair<size_t, size_t>> m_WinCountsTable;

    public:
        Game()
        {
        }

        static constexpr uint8_t EndPos(uint8_t StartPos, int RollValue)
        {
            // Work with 0-based index
            int startIndex = StartPos - 1;
            int endIndex = (startIndex + RollValue) % 10;

            // Convert back to 1-based
            return uint8_t(endIndex + 1);
        }

        // Returns an end-position value for a given universe index
        uint8_t QuantumTurn(uint8_t Position, uint8_t Universe)
        {
            // Construct a unique key from the possible input values
            uint32_t key = Position + (uint32_t(Universe) << 8) ;
            auto it = m_EndPosTable.find(key);
            if (it == m_EndPosTable.end())
            {
                // Do the math
                uint8_t rollResult;
                rollResult = EndPos(Position, RollResults[Universe]);
                m_EndPosTable.emplace(key, rollResult);
                return rollResult;
            }

            return it->second;
        }

        // Returns the number of possible wins for each player as a pair
        std::pair<size_t, size_t> Simulate(const std::pair<uint8_t, uint8_t> &Positions, std::pair<uint8_t, uint8_t> Scores)
        {
            std::pair<size_t, size_t> WinCounts(0, 0);
            uint32_t key = Positions.first + (uint32_t(Positions.second) << 8) + (uint32_t(Scores.first) << 16) + (uint32_t(Scores.second) << 24);
            auto it = m_WinCountsTable.find(key);
            if (it == m_WinCountsTable.end())
            {
                // Do the math
                for (uint8_t i = 0; i < NewUniversesPerTurn; ++i)
                {
                    std::pair<uint8_t, uint8_t> newPositions(QuantumTurn(Positions.first, i), Positions.second);
                    std::pair<uint8_t, uint8_t> newScores(uint8_t(Scores.first + newPositions.first), Scores.second);

                    if (newScores.first >= WinningScore)
                    {
                        WinCounts.first++;
                    }
                    else
                    {
                        for (uint8_t j = 0; j < NewUniversesPerTurn; ++j)
                        {
                            newPositions.second = QuantumTurn(Positions.second, j);
                            newScores.second = Scores.second + newPositions.second;

                            if (newScores.second >= WinningScore)
                            {
                                WinCounts.second++;
                            }
                            else
                            {
                                std::pair<size_t, size_t> InnerWinCounts = Simulate(newPositions, newScores);
                                WinCounts.first += InnerWinCounts.first;
                                WinCounts.second += InnerWinCounts.second;
                            }
                        }
                    }
                }
                m_WinCountsTable.emplace(key, WinCounts);
            }
            else
            {
                WinCounts = it->second;
            }

            return WinCounts;
        }
    };

    void Execute()
    {
        Game game;
        std::pair<size_t, size_t> WinCounts = game.Simulate(std::pair(Player1Start, Player2Start), std::make_pair<uint8_t, uint8_t>(0, 0));
        std::cout << "Player 1 Wins: " << WinCounts.first << std::endl;
        std::cout << "Player 2 Wins: " << WinCounts.second << std::endl;
    }
}