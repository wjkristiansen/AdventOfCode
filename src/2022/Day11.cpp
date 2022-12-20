#include "../pch.h"
#include "../SolutionFactory.h"

enum class OpType
{
    AddConstant,
    MulConstant,
    Double,
    Square,
};

template<OpType Type, typename ValueType> struct Operation;

template<typename ValueType>
struct OperationBase
{
    virtual ~OperationBase() = default;
    virtual ValueType Execute(const ValueType &old) const = 0;
};

template <typename ValueType>
struct Operation<OpType::AddConstant, ValueType> : public OperationBase<ValueType>
{
    ValueType Constant;
    Operation(ValueType c) : Constant(c) {}
    virtual ValueType Execute(const ValueType &old) const final { return old + Constant; }
};

template <typename ValueType>
struct Operation<OpType::MulConstant, ValueType> : public OperationBase<ValueType>
{
    ValueType Constant;
    Operation(ValueType c) : Constant(c) {}
    virtual ValueType Execute(const ValueType &old) const final { return old * Constant; }
};

template <typename ValueType>
struct Operation<OpType::Double, ValueType> : public OperationBase<ValueType>
{
    virtual ValueType Execute(const ValueType &old) const final { return old + old; }
};

template <typename ValueType>
struct Operation<OpType::Square, ValueType> : public OperationBase<ValueType>
{
    virtual ValueType Execute(const ValueType &old) const final { ValueType result = old * old; assert(old == result / old); return result; }
};

using ValueType = uint64_t;

struct Monkey
{
    ValueType TestDivisor = 1U;
    int TrueMonkey = 0;
    int FalseMonkey = 0;
    std::unique_ptr<OperationBase<ValueType>>pOperation;
    std::deque<ValueType> Items;
    size_t InspectCount = 0;

    void DoInspection(std::vector<Monkey> &Monkeys, const ValueType &lcm)
    {
        for(auto Item : Items)
        {
            InspectCount++;
            Items.pop_front();
            ValueType newLevel = pOperation->Execute(Item);
//            newLevel /= 3;
            newLevel %= lcm;
            bool test = newLevel == TestDivisor * (newLevel / TestDivisor);
            if(test)
                Monkeys[TrueMonkey].Items.push_back(newLevel);
            else
                Monkeys[FalseMonkey].Items.push_back(newLevel);
        }
    }
};

void CSolution<2022, 11>::Execute(int)
{
    std::ifstream fstream("Day11.txt");
    std::vector<Monkey> Monkeys;

    ValueType lcm = 1;

    for(;!fstream.eof();)
    {
        std::string line;
        std::getline(fstream, line); // "Monkey <n>:"
        if(line.empty())
            break;
        Monkeys.emplace_back();
        Monkey& monkey = Monkeys.back();
        std::string token;
        fstream >> token; // "Starting"
        fstream >> token; // "items:"

        for (;;)
        {
            int value;
            fstream >> value;
            monkey.Items.push_back(ValueType(value));
            char c;
            fstream >> c;
            if (c != ',')
            {
                fstream.unget();
                break;
            }
        }

        std::string opString;
        fstream >> token; // "Operation:"
        fstream >> token; // "new"
        fstream >> token; // "="
        fstream >> token; // "old"
        fstream >> opString; // "+" or "-"
        fstream >> token; // "old" or some constant integer
        if (token == "old")
        {
            if (opString == "+")
            {
                monkey.pOperation = std::make_unique<Operation<OpType::Double, ValueType>>();
            }
            else
            {
                monkey.pOperation = std::make_unique<Operation<OpType::Square, ValueType>>();
            }
        }
        else
        {
            int constant = std::stoi(token);

            if (opString == "+")
            {
                monkey.pOperation = std::make_unique<Operation<OpType::AddConstant, ValueType>>(constant);
            }
            else
            {
                monkey.pOperation = std::make_unique<Operation<OpType::MulConstant, ValueType>>(constant);
            }
        }

        ValueType divisor;
        fstream >> token; // "Test:"
        fstream >> token; // "divisible"
        fstream >> token; // "by"
        fstream >> divisor;
        lcm *= divisor;

        monkey.TestDivisor = divisor;

        int throwMonkey;
        fstream >> token; // "If"
        fstream >> token; // "true:"
        fstream >> token; // "throw"
        fstream >> token; // "to"
        fstream >> token; // "monkey"
        fstream >> throwMonkey;
        monkey.TrueMonkey = throwMonkey;

        fstream >> token; // "If"
        fstream >> token; // "false:"
        fstream >> token; // "throw"
        fstream >> token; // "to"
        fstream >> token; // "monkey"
        fstream >> throwMonkey;
        monkey.FalseMonkey = throwMonkey;

        std::getline(fstream, line); // Rest of line
        std::getline(fstream, line); // Blank line
    }

    for (int round = 0; round < 10000 ; ++round)
    {
        for (auto& monkey : Monkeys)
        {
            monkey.DoInspection(Monkeys, lcm);
        }
    }

    size_t Max = 0;
    size_t Max2 = 0;
    for (int i = 0; i < Monkeys.size(); ++i)
    {
        std::cout << "Monkey " << i << " inspected items " << Monkeys[i].InspectCount << " times." << std::endl;
        if (Monkeys[i].InspectCount > Max)
        {
            Max2 = Max;
            Max = Monkeys[i].InspectCount;
        }
        else if (Monkeys[i].InspectCount > Max2)
        {
            Max2 = Monkeys[i].InspectCount;
        }
    }

    std::cout << "Top two: " << Max << " and " << Max2 << std::endl;

    size_t MonkeyBusiness = Max * Max2;

    std::cout << "Monkey Business: " << MonkeyBusiness << std::endl;
}