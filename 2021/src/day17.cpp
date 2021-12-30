#include <iostream>
#include <vector>
#include <set>

#include "day17.h"

namespace Day17
{
    struct vec
    {
        int x, y;
        vec(int X, int Y) :
            x(X),
            y(Y) {}
    };
    struct rect
    {
        vec min;
        vec max;
        rect(int minX, int minY, int maxX, int maxY) :
            min(minX, minY),
            max(maxX, maxY) {}
    };

//    rect Target(20, -10, 30, -5);
    rect Target(269, -68, 292, -44);

    void Execute()
    {
        // What goes up must come down, with Y-velocity equal
        // to -1 * Vy0. The next step after puts the velocity
        // at Vy0 + 1.

        // Assuming the target is below the probe origin, the
        // fastest the probe can move without passing through
        // the target is -Target.min.y. Also, since the Y-velocity
        // will increase by 1 after passing back through the origin,
        // we can say Vy0 + 1 < -Target.min.y, or
        // Vy0 < -Target.min.y - 1.

        // Given the example set x=20..30, y=-10..-5, The fastest Vy0 is (-10 + 1)
        // Target.min.y is -10 so -Target.min.y - 1 is 9. This matches the
        // Vy0 given for the sample data.

        // The peak altitude is going to be the sum of all integers between
        // 1 and Vy0 (e.g. if Vy0 is 5 then the peak will be 5 + 4 + 3 + 2 + 1 = 15).
        // A useful formula for summing all integers from 1 to N is:
        // (N * (N + 1))/2
        // (5 * (5 + 1))/2 = 15

        // Therefore, we can give a super-simple answer for part 1:
        int N = (-Target.min.y) - 1;
        int Peak = (N * (N + 1)) / 2;
        std::cout << "Peak=" << Peak << std::endl;

        // Find initial values of Vx which produce an X position
        // between minX and maxX inclusive

        std::set<std::pair<int, int>> Solutions;

        int MaxXVel = Target.max.x;
        int MinXVel = MaxXVel;
        for (int V0 = MaxXVel; V0 > 0; --V0)
        {
            int Step = 0;
            int X = 0;
            int Vx = V0;
            for (;;)
            {
                Step++;
                X += Vx;
                if (X > Target.max.x)
                    break; // Beyond range

                Vx -= 1;
                if (Vx == 0)
                    break; // Ceased progress
            }

            if (X <= Target.max.x && X >= Target.min.x)
            {
                MinXVel = std::min(MinXVel, V0);
            }
        }

        // Store all possible Y values in a priority queue starting with max Y
        int MaxYVel = Target.min.y > 0 ? Target.max.y : -(Target.min.y) - 1;
        for (int Vy0 = MaxYVel; Vy0 >= Target.min.y; Vy0--)
        {
            for (int Vx0 = MaxXVel; Vx0 >= MinXVel; Vx0--)
            {
                int Y = 0;
                int X = 0;
                int Vy = Vy0;
                int Vx = Vx0;
                for (int Step = 1; ; ++Step)
                {
                    Y += Vy;
                    X += Vx;
                    Vy -= 1;
                    Vx = std::max(0, Vx - 1);

                    if (Vy < 0 && Y < Target.min.y)
                        break;
                    if (X > Target.max.x)
                        break;

                    if (Y >= Target.min.y && Y <= Target.max.y && X >= Target.min.x)
                    {
                        auto it = Solutions.emplace(Vx0, Vy0);
                        if (it.second)
                        {
                            std::cout << Vx0 << ", " << Vy0 << std::endl;
                        }
                    }
                }
            }
        }
        std::cout << "NumSolutions=" << Solutions.size() << std::endl;
    }
}
