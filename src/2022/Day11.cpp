#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <vector>
#include <memory>
#include <assert.h>

#include "..\SolutionFactory.h"

enum class OpType
{
    AddConstant,
    MulConstant,
    Double,
    Square,
};

template<OpType Type>
struct Operation
{
};

struct OperationBase
{
    virtual ~OperationBase() = default;
    virtual int Execute(int old) const = 0;
};

template <>
struct Operation<OpType::AddConstant> : public OperationBase
{
    int Constant;
    Operation(int c) : Constant(c) {}
    virtual int Execute(int old) const final { return old + Constant; }
};

template <>
struct Operation<OpType::MulConstant> : public OperationBase
{
    int Constant;
    Operation(int c) : Constant(c) {}
    virtual int Execute(int old) const final { return old * Constant; }
};

template <>
struct Operation<OpType::Double> : public OperationBase
{
    virtual int Execute(int old) const final { return old + old; }
};

template <>
struct Operation<OpType::Square> : public OperationBase
{
    virtual int Execute(int old) const final { return old * old; }
};

struct Monkey
{
    int TestDivisor = 1;
    int TrueMonkey = 0;
    int FalseMonkey = 0;
    std::unique_ptr<OperationBase> pOperation;
    std::deque<int> Items;
    int InspectCount = 0;

    void DoInspection(std::vector<Monkey> &Monkeys)
    {
        for(auto Item : Items)
        {
            InspectCount++;
            Items.pop_front();
            int newLevel = pOperation->Execute(Item);
            newLevel /= 3;
            bool test = newLevel == TestDivisor * (newLevel / TestDivisor);
            if(test)
                Monkeys[TrueMonkey].Items.push_back(newLevel);
            else
                Monkeys[FalseMonkey].Items.push_back(newLevel);
        }
    }
};

void CSolution<2022, 11>::Execute()
{
    std::ifstream fstream("Day11.txt");
    std::vector<Monkey> Monkeys;

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
            monkey.Items.push_back(value);
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
                monkey.pOperation = std::make_unique<Operation<OpType::Double>>();
            }
            else
            {
                monkey.pOperation = std::make_unique<Operation<OpType::Square>>();
            }
        }
        else
        {
            int constant = std::stoi(token);

            if (opString == "+")
            {
                monkey.pOperation = std::make_unique<Operation<OpType::AddConstant>>(constant);
            }
            else
            {
                monkey.pOperation = std::make_unique<Operation<OpType::MulConstant>>(constant);
            }
        }

        int divisor;
        fstream >> token; // "Test:"
        fstream >> token; // "divisible"
        fstream >> token; // "by"
        fstream >> divisor;

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

    for (int round = 0; round < 20; ++round)
    {
        for (auto& monkey : Monkeys)
        {
            monkey.DoInspection(Monkeys);
        }
    }

    int Max = 0;
    int Max2 = 0;
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

    int MonkeyBusiness = Max * Max2;

    std::cout << "Monkey Business: " << MonkeyBusiness << std::endl;
}