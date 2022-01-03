#include <iostream>
#include <vector>

#include "day21.h"

namespace Day21
{
    struct Player
    {
        size_t Score = 0;
        int Position = 0;
    };

    class Game
    {
        int m_DetValueLast = 0;
        int m_NextPlayer = 0;
        int m_RollCount = 0;
        Player m_Players[2];

    public:
        Game(int p1, int p2)
        {
            m_Players[0].Position = p1;
            m_Players[1].Position = p2;
        }
        int Roll()
        {
            m_RollCount += 3;
            int result = 0;
            m_DetValueLast = (m_DetValueLast % 100) + 1;
            result += m_DetValueLast;
            m_DetValueLast = (m_DetValueLast % 100) + 1;
            result += m_DetValueLast;
            m_DetValueLast = (m_DetValueLast % 100) + 1;
            result += m_DetValueLast;
            return result;
        }
        
        int Turn()
        {
            int move = Roll();
            m_Players[m_NextPlayer].Position = (((m_Players[m_NextPlayer].Position - 1) + move) % 10) + 1;
            m_Players[m_NextPlayer].Score += m_Players[m_NextPlayer].Position;
            if( m_Players[m_NextPlayer].Score >= 1000 )
                return m_NextPlayer;
            m_NextPlayer = m_NextPlayer ^ 1;
            return -1;
        }

        void Run()
        {
            int Winner = -1;
            while (Winner == -1) { Winner = Turn(); }
            size_t Product = m_Players[(Winner ^ 1)].Score * m_RollCount;
            std::cout << "Roll Count: " << m_RollCount << std::endl;
            std::cout << "Player 1 Score: " << m_Players[0].Score << std::endl;
            std::cout << "Player 2 Score: " << m_Players[1].Score << std::endl;
            std::cout << "Product: " << Product << std::endl;
        } 
    };

    void Execute()
    {
        Game game(6, 9);
        game.Run();
    }
}