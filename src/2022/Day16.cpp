#include "../pch.h"
#include "../SolutionFactory.h"

struct ValveNode
{
    ValveNode(const std::string &name, int flowRate, std::vector<size_t> neighborIndices) :
        Name(name),
        FlowRate(flowRate),
        NeighborIndices(neighborIndices)
    {
    }

    const std::string Name;
    const int FlowRate;
    
    size_t BestFlowRate = FlowRate;

    std::vector<size_t> NeighborIndices;
};

struct ValveMap
{
    std::unordered_map<std::string, size_t> NameToIndex;
    std::vector<std::unique_ptr<ValveNode>> ValveNodes;
    std::deque<size_t> ScanQueue;

    size_t DeclareValve(const std::string &name)
    {
        auto it = NameToIndex.find(name);
        if(it != NameToIndex.end())
            return it->second; // Already declared

        size_t Index = ValveNodes.size();
        ValveNodes.emplace_back(nullptr); // Emplace empty node
        NameToIndex.emplace(name, Index);
        return Index;
    }

    ValveNode *CreateValveNode(const std::string &name, int flowRate, const std::vector<std::string> &NeighborNames)
    {
        size_t Index = DeclareValve(name);
        if(ValveNodes[Index])
            throw std::invalid_argument(std::string("Valve ") + name + std::string(" already created"));

        // Get neighbor indices
        std::vector<size_t> NeighborIndices(NeighborNames.size());
        for(size_t i = 0; i < NeighborNames.size(); ++i)
        {
            NeighborIndices[i] = DeclareValve(NeighborNames[i]);
        }

        ValveNodes[Index] = std::make_unique<ValveNode>(name, flowRate, NeighborIndices);

        return ValveNodes[Index].get();
    }

    ValveNode *GetValveNodeByIndex(size_t Index) const
    {
        return Index < ValveNodes.size() ? ValveNodes[Index].get() : nullptr;
    }

    ValveNode *GetValveNodeByName(const std::string &name) const
    {
        auto it = NameToIndex.find(name);
        if(it == NameToIndex.end())
            return nullptr;
        
        return ValveNodes[it->second].get();
    }

    int Scan(int /*minutesLeft*/, size_t index)
    {
        std::vector<bool> Visited(ValveNodes.size(), false);
        Visited[index] = true;
        ScanQueue.push_front(index);

        while (!ScanQueue.empty())
        {
            size_t cur = ScanQueue.back();
            ScanQueue.pop_back();

            ValveNode* pNode = ValveNodes[cur].get();
            std::cout << pNode->Name << std::endl;

            // For each neighbor of node `n`...
            for (auto n : pNode->NeighborIndices)
            {
                if (!Visited[n])
                {
                    Visited[n] = true;
                    ScanQueue.push_front(n);
                }
            }
        }

        return 0;
    }
};

void CSolution<2022, 16>::Execute(int)
{
    ValveMap Map;
    std::ifstream fstream("Day16.txt");

    for(;!fstream.eof();)
    {
        std::string line;
        std::getline(fstream, line);
        if(line.empty())
            break;
        std::istringstream ss(line);

        std::string Name;
        int FlowRate;
        std::vector<std::string> NeighborNames;

        ss.ignore(line.length(), ' ');
        ss >> Name;
        ss.ignore(line.length(), '=');
        ss >> FlowRate;
        ss.ignore(line.length(), ' '); // ";"
        ss.ignore(line.length(), ' '); // "tunnels"
        ss.ignore(line.length(), ' '); // "lead"
        ss.ignore(line.length(), ' '); // "to"
        ss.ignore(line.length(), ' '); // "valves"

        while(!ss.eof())
        {
            char neighborName[3];
            ss.get(neighborName, 3);
            NeighborNames.push_back(neighborName);
            ss.ignore(line.length(), ' ');
        }

        Map.CreateValveNode(Name, FlowRate, NeighborNames);
    }

    // Use BFS to find optimal sequence
    auto x = Map.Scan(10, 0);
    std::cout << x << std::endl;
}
