#include "pch.h"
#include "SolutionFactory.h"

class CSolution2: public CSolutionBase
{
public:
    CSolution2() : CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        int minPart = part > 0 ? part : 1;
        int maxPart = part > 0 ? part : 2;
        for (int runPart = minPart; runPart <= maxPart; ++runPart)
        {
            if (runPart == 1)
            {
                std::ifstream fstream("2024/Day2.txt");
                std::string line;
                int safeCount = 0;

                for (;!fstream.eof();)
                {
                    std::getline(fstream, line);
                    if (line.empty())
                        break;
                    std::istringstream ss(line);
                    int lastVal;
                    ss >> lastVal;
                    int lastDelta = 0;
                    bool safe = true;
                    while (!ss.eof())
                    {
                        int val;
                        ss >> val;
                        int delta = val - lastVal;
                        if (delta == 0 || std::abs(delta) > 3)
                        {
                            safe = false;
                            break;
                        }
                        if (delta < 0 && lastDelta > 0)
                        {
                            safe = false;
                            break;
                        }
                        if (delta > 0 && lastDelta < 0)
                        {
                            safe = false;
                            break;
                        }
                        lastVal = val;
                        lastDelta = delta;
                    }

                    if(safe)
                        safeCount++;

                }

                std::cout << "Part 1: Safe count: " << safeCount << std::endl;
            }
            else
            {
                std::ifstream fstream("2024/Day2.txt");
                std::string line;
                int safeCount = 0;
                int reportCount = 0;

                for (;!fstream.eof(); ++reportCount)
                {
                    std::vector<int> values;
                    {
                        std::getline(fstream, line);
                        if (line.empty())
                            break;
                        std::istringstream ss(line);
                        while (!ss.eof())
                        {
                            int v;
                            ss >> v;
                            values.push_back(v);
                        }
                    }

                    for (int ignoreIndex = -1; ignoreIndex < int(values.size()); ignoreIndex++)
                    {
                        bool safe = true;
                        int lastVal = -1;
                        int lastDelta = 0;
                        for (int index = 0; index < int(values.size()); ++index)
                        {
                            if (index == ignoreIndex)
                                continue;
                            int val = values[index];
                            if (lastVal >= 0)
                            {
                                int delta = val - lastVal;
                                if (delta == 0 || std::abs(delta) > 3)
                                {
                                    safe = false;
                                    break;
                                }
                                if (delta < 0 && lastDelta > 0)
                                {
                                    safe = false;
                                    break;
                                }
                                if (delta > 0 && lastDelta < 0)
                                {
                                    safe = false;
                                    break;
                                }
                                lastDelta = delta;
                            }
                            lastVal = val;
                        }

                        if (safe)
                        {
                            safeCount++;
                            break;
                        }
                    }
                }

                std::cout << "Part 2: Safe count: " << safeCount << std::endl;
            }
        }
    }
};

DECLARE_SOLUTION(CSolution2, 2, "Red-Nosed Reports");
