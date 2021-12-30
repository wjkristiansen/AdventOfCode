#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

#include "day18.h"

namespace Day18
{
    class SnailfishNumber
    {
        int Value = 0;
        std::unique_ptr<SnailfishNumber> pLeft;
        std::unique_ptr<SnailfishNumber> pRight;
        SnailfishNumber* pParent = nullptr;

        size_t FromString(const std::string& str, size_t offset)
        {
            size_t result = 0;
            if (str[offset] == '[')
            {
                // Begin a pair
                pLeft = std::make_unique<SnailfishNumber>();
                pLeft->pParent = this;
                offset = pLeft->FromString(str, offset + 1);
                if (str[offset] != ',')
                    throw(std::exception());
                pRight = std::make_unique <SnailfishNumber>();
                pRight->pParent = this;
                offset = pRight->FromString(str, offset + 1);
                if (str[offset] != ']')
                    throw(std::exception());
                result = offset + 1; // Move past ']'
            }
            else
            {
                // Assume a single-digit number
                Value = int(str[offset] - '0');
                if (Value < 0 || Value > 9)
                    throw(std::exception());
                result = offset + 1;
            }

            return result;
        }

        SnailfishNumber* LeftMost()
        {
            if (!IsPair())
                return this;

            return pLeft->LeftMost();
        }

        SnailfishNumber* RightMost()
        {
            if (!IsPair())
                return this;

            return pRight->RightMost();
        }

        SnailfishNumber* SeekLeftReg(SnailfishNumber* pSource)
        {
            if (!IsPair())
                return this;

            SnailfishNumber* pNumber = nullptr;

            if (pSource == pLeft.get() && pParent)
            {
                // Continue up the chain
                pNumber = pParent->SeekLeftReg(this);
            }
            else if (pSource == pRight.get())
            {
                // Continue down the right side of the left element
                pNumber = pLeft->RightMost();
            }
            else if (pSource == pParent)
            {
                // Search for the right side of the right element
                pNumber = pRight->RightMost();
            }

            return pNumber;
        }

        SnailfishNumber* SeekRightReg(SnailfishNumber* pSource)
        {
            if (!IsPair())
                return this;

            SnailfishNumber* pNumber = nullptr;

            if (pSource == pRight.get() && pParent)
            {
                // Continue up the chain
                pNumber = pParent->SeekRightReg(this);
            }
            else if (pSource == pLeft.get())
            {
                // Continue down the left side of the right element
                pNumber = pRight->LeftMost();
            }
            else if (pSource == pParent)
            {
                // Search for the left side of the left element
                pNumber = pLeft->LeftMost();
            }

            return pNumber;
        }

        bool IsPair() const { return pLeft.get() != nullptr; }

        bool Explode(int depth)
        {
            if (IsPair())
            {
                if (depth == 4)
                {
                    // Explode
                    assert(!pLeft->IsPair());
                    assert(!pRight->IsPair());

                    SnailfishNumber* pLeftReg = pParent->SeekLeftReg(this);
                    if (pLeftReg)
                        pLeftReg->Value += pLeft->Value;

                    SnailfishNumber* pRightReg = pParent->SeekRightReg(this);
                    if (pRightReg)
                        pRightReg->Value += pRight->Value;

                    pLeft = nullptr;
                    pRight = nullptr;

                    Value = 0;

                    // Indicate an explode occurred
                    return true;
                }
                else
                {
                    if (pLeft->Explode(depth + 1))
                        return true;
                    if (pRight->Explode(depth + 1))
                        return true;
                }
            }

            return false;
        }

        bool Split()
        {
            if (IsPair())
            {
                // Attempt to split the left, then the right
                if (pLeft->Split())
                    return true;
                if (pRight->Split())
                    return true;
            }
            else
            {
                if (Value >= 10)
                {
                    // Allocate a new pair of nodes
                    int LeftValue = Value / 2;
                    int RightValue = Value - LeftValue;
                    pLeft = std::make_unique<SnailfishNumber>(LeftValue);
                    pLeft->pParent = this;
                    pRight = std::make_unique <SnailfishNumber>(RightValue);
                    pRight->pParent = this;

                    return true;
                }
            }

            return false;
        }

        void Reduce()
        {
            // Should only be called on the root number
            assert(pParent == nullptr);

            for (;;)
            {
                if (Explode(0))
                    continue;
                if (Split())
                    continue;
                break;
            }
        }

    public:
        SnailfishNumber(const SnailfishNumber& o)
        {
            if (o.IsPair())
            {
                // Deep copy
                pLeft = std::make_unique<SnailfishNumber>(*o.pLeft);
                pLeft->pParent = this;
                pRight = std::make_unique<SnailfishNumber>(*o.pRight);
                pRight->pParent = this;
            }
            else
            {
                Value = o.Value;
            }
        }

        SnailfishNumber(int value = 0) :
            Value(value) {}

        SnailfishNumber(const std::string& str)
        {
            FromString(str, 0);

            Reduce();
        }

        int Magnitude()
        {
            if (IsPair())
            {
                return 3 * pLeft->Magnitude() + 2 * pRight->Magnitude();
            }

            return Value;
        }

        SnailfishNumber& operator=(const SnailfishNumber& o)
        {
            if (o.IsPair())
            {
                // Deep copy
                pLeft = std::make_unique<SnailfishNumber>(*o.pLeft);
                pLeft->pParent = this;
                pRight = std::make_unique<SnailfishNumber>(*o.pRight);
                pRight->pParent = this;
            }
            else
            {
                Value = o.Value;
            }

            Reduce();
            
            return *this;
        }

        SnailfishNumber operator+(const SnailfishNumber& o) const
        {
            SnailfishNumber Sum;

            Sum.pLeft = std::make_unique<SnailfishNumber>(*this);
            pLeft->pParent = &Sum;
            Sum.pRight = std::make_unique<SnailfishNumber>(o);
            pRight->pParent = &Sum;

            return Sum;
        }

        std::string ToString()
        {
            if (IsPair())
            {
                return std::string("[" + pLeft->ToString() + "," + pRight->ToString() + "]");
            }
            else
            {
                return std::to_string(Value);
            }
        }
    };

    void Execute()
    {
        SnailfishNumber Sum(Day18::Numbers[0]);
        std::cout << Sum.ToString() << std::endl;
        for (size_t i = 1; i < _countof(Numbers); ++i)
        {
            SnailfishNumber Next(Day18::Numbers[i]);
            std::cout << Next.ToString() << std::endl;
            Sum = Sum + Next;
        }

        std::cout << "Final sum: " << Sum.ToString() << std::endl;
        std::cout << "Magnitude: " << Sum.Magnitude() << std::endl;
    }
}