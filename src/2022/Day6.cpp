#include "pch.h"
#include "SolutionFactory.h"

class CSolution6: public CSolutionBase
{
    bool AreAllUnique(const std::deque<char>& queue)
    {
        std::set<char> uniqueChars;
        uniqueChars.insert(queue.begin(), queue.end());
        if(uniqueChars.size() == queue.size())
            return true; // All four chars are unique

        return false;
    }

    void Execute(int)
    {
        std::ifstream fstream("2022/Day6.txt");
        std::deque<char> queue;

        // Find the first location in the stream of 4 unique characters
        std::optional<size_t> start;
        std::optional<size_t> messageStart;
        for(size_t pos = 0; !fstream.eof(); ++pos)
        {
            char c;
            fstream >> c;
            queue.push_back(c);

            if (!messageStart.has_value())
            {
                if (queue.size() == 14)
                {
                    if (AreAllUnique(queue))
                    {
                        messageStart = pos;
                        break;
                    }

                    queue.pop_front();
                } 
            }
            else
            {
                if (!start.has_value())
                {
                    if (queue.size() == 4)
                    {
                        if (AreAllUnique(queue))
                        {
                            start = pos;
                            break;
                        }

                        queue.pop_front();
                    }
                }
            }
        }

        if(messageStart.has_value())
        {
            std::cout << messageStart.value() + 1 << std::endl;
        }
    }
};

DECLARE_SOLUTION(CSolution6, 6, "Tuning Trouble");