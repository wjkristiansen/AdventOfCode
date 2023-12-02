#include "pch.h"
#include "SolutionFactory.h"

void CSolution<1>::Execute(int part)
{
    if (part == 1)
    {
        std::ifstream fstream("2023/Day1p1.txt");
        std::string line;
        int sum = 0;

        for (;;)
        {
            std::getline(fstream, line);
            if (line.empty())
                break;

            int first = 0;
            int last = 0;

            // Find the first digit
            for (auto it = line.begin(); it != line.end(); ++it)
            {
                if (*it >= '0' && *it <= '9')
                {
                    first = *it - '0';
                    break;
                }
            }

            // Find the last digit
            for (auto it = line.rbegin(); it != line.rend(); ++it)
            {
                if (*it >= '0' && *it <= '9')
                {
                    last = *it - '0';
                    break;
                }
            }

            int number = first * 10 + last;
            sum += number;
        }

        std::cout << "Calibration sum: " << sum << std::endl;
    }
    else
    {
        std::ifstream fstream("2023/Day1p2.txt");
        std::string line;
        int sum = 0;

        struct Digit
        {
            std::string name;
            char c;
            int value;
        };

        Digit digits[] =
        {
            Digit{"zero", '0', 0},
            Digit{"one", '1', 1},
            Digit{"two", '2', 2},
            Digit{"three", '3', 3},
            Digit{"four", '4', 4},
            Digit{"five", '5', 5},
            Digit{"six", '6', 6},
            Digit{"seven", '7', 7},
            Digit{"eight", '8', 8},
            Digit{"nine", '9', 9},
        };

        for (;;)
        {
            std::getline(fstream, line);
            if (line.empty())
                break;

            int first = 0;
            int last = 0;
            int firstPos = INT_MAX;
            int lastPos = -1;
            size_t pos;

            // Find the first digit
            for (auto digit : digits)
            {
                pos = line.find(digit.name);
                if (pos != std::string::npos && int(pos) < firstPos)
                {
                    first = digit.value;
                    firstPos = int(pos);
                }

                pos = line.find_first_of(digit.c);
                if (pos != std::string::npos && int(pos) < firstPos)
                {
                    first = digit.value;
                    firstPos = int(pos);
                }

                if (firstPos == 0)
                    break;
            }

            // Find the last digit
            for (auto digit : digits)
            {
                pos = line.rfind(digit.name);
                if (pos != std::string::npos && int(pos) > lastPos)
                {
                    last = digit.value;
                    lastPos = int(pos);
                }

                pos = line.find_last_of(digit.c);
                if (pos != std::string::npos && int(pos) > lastPos)
                {
                    last = digit.value;
                    lastPos = int(pos);
                }
            }

            int number = first * 10 + last;
            sum += number;
        }

        std::cout << "Calibration sum: " << sum << std::endl;
    }
}
