#include "pch.h"
#include "SolutionFactory.h"

enum class NodeType
{
    Int,
    List,
};

struct NodeBase
{
    virtual NodeType Type() const = 0;
    virtual int Compare(const struct NodeBase& o) const = 0;
    virtual std::string ToString() const = 0;
    virtual ~NodeBase() {}
};

struct ListNode : public NodeBase
{
    std::vector<std::unique_ptr<NodeBase>> ChildNodes;

    virtual NodeType Type() const { return NodeType::List;  }
    virtual int Compare(const NodeBase& o) const;
    virtual std::string ToString() const
    {
        std::ostringstream ss;
        for (auto it = ChildNodes.begin(); it != ChildNodes.end();)
        {
            ss << it->get()->ToString();
            ++it;
            if (it != ChildNodes.end())
                ss << ",";
        }

        return ss.str();
    }

    int Compare(const struct IntNode& o) const;
    int Compare(const struct ListNode& o) const;
};

struct InnerListNode : public ListNode
{
    virtual std::string ToString() const
    {
        return std::string("[") + ListNode::ToString() + std::string("]");
    }
};

struct IntNode : public NodeBase
{
    IntNode() = delete;

    IntNode(int value) : Value(value) {}
    int Value;

    virtual NodeType Type() const { return NodeType::Int; }
    virtual int Compare(const NodeBase& o) const;
    virtual std::string ToString() const
    {
        std::ostringstream ss;
        ss << Value;
        return ss.str();
    }

    int Compare(const struct IntNode& o) const;
    int Compare(const struct ListNode& o) const;
};

int IntNode::Compare(const IntNode& o) const
{
    if (Value < o.Value)
        return -1;

    if (Value > o.Value)
        return 1;

    return 0;
}

int IntNode::Compare(const ListNode& o) const
{
    ListNode temp;
    temp.ChildNodes.emplace_back(std::make_unique<IntNode>(Value));
    return temp.Compare(o);
}

int IntNode::Compare(const NodeBase& o) const
{
    //std::cout << "Compare " << ToString() << " vs " << o.ToString() << std::endl;

    switch (o.Type())
    {
    case NodeType::Int:
        return Compare(reinterpret_cast<const IntNode&>(o));
        break;

    case NodeType::List:
        return Compare(reinterpret_cast<const ListNode&>(o));
        break;
    }

    return false;
}

int ListNode::Compare(const IntNode& o) const
{
    ListNode temp;
    temp.ChildNodes.emplace_back(std::make_unique<IntNode>(o.Value));
    return Compare(temp);
}

int ListNode::Compare(const ListNode& o) const
{
    auto itLeft = ChildNodes.begin();
    auto itRight = o.ChildNodes.begin();

    for (; itLeft != ChildNodes.end() && itRight != o.ChildNodes.end(); ++itLeft, ++itRight)
    {
        int Compare = itLeft->get()->Compare(*itRight->get());
        if(Compare != 0)
            return Compare;
    }

    if (itLeft == ChildNodes.end() && itRight != o.ChildNodes.end())
        return -1; // Left side ran out of items

    if (itRight == o.ChildNodes.end() && itLeft != ChildNodes.end())
        return 1; // Right side ran out of items

    return 0;
}

int ListNode::Compare(const NodeBase& o) const
{
    //std::cout << "Compare " << ToString() << " vs " << o.ToString() << std::endl;

    switch (o.Type())
    {
    case NodeType::Int:
        return Compare(reinterpret_cast<const IntNode&>(o));
        break;

    case NodeType::List:
        return Compare(reinterpret_cast<const ListNode&>(o));
        break;
    }

    return 0;
}
class CSolution13: public CSolutionBase
{
    static void ReadNode(std::istream &stream, ListNode *pParent)
    {
        std::optional<int> Value;
        char c;
        for(stream >> c; !stream.eof(); stream >> c)
        {
            switch(c)
            {
            case '[': {
                ListNode * pChild = new InnerListNode();
                ReadNode(stream, pChild);
                pParent->ChildNodes.emplace_back(pChild);
                break; }

            case ']':
                if(Value.has_value())
                {
                    NodeBase *pChild = new IntNode(Value.value());
                    pParent->ChildNodes.emplace_back(pChild);
                }
                return;

            case ',':
                if (Value.has_value())
                {
                    NodeBase *pChild = new IntNode(Value.value());
                    pParent->ChildNodes.emplace_back(pChild);
                }
                Value.reset();
                break;

            default:
                if(Value.has_value())
                    Value = Value.value() * 10 + (c - '0');
                else
                    Value = c - '0';
                break;
            }
        }
    }

    struct Packet
    {
        std::unique_ptr<NodeBase> Data;

        Packet() = delete;
        Packet(const Packet &) = delete;
        Packet(Packet &&o) :
            Data(std::move(o.Data))
        {
        }

        Packet(const std::string &s)
        {
            std::istringstream ss(s);
            auto pNode = new ListNode();
            ReadNode(ss, pNode);
            Data = std::unique_ptr<NodeBase>(pNode);
        }

        bool operator<(const Packet &o) const
        {
            return Data->Compare(*o.Data) < 0;
        }

        std::string ToString() const { return Data->ToString(); }
    };

    void Execute(int)
    {
        std::set<Packet> Packets;
        std::ifstream fstream("Day13.txt"); 

        for(;!fstream.eof();)
        {
            std::string line;
            std::getline(fstream, line);

            if(!line.empty())
            {
                Packets.emplace(line);
            }
        }

        // Now add the "divider packets"
        const std::string DividerPacketStrings[] =
        {
            "[[2]]",
            "[[6]]"
        };
        Packets.emplace(DividerPacketStrings[0]);
        Packets.emplace(DividerPacketStrings[1]);

        size_t DividerPacketIndices[2];

        // Dump the ordered packets
        size_t Index = 1;
        for(auto &Packet : Packets)
        {
            std::string s = Packet.ToString();

            if (s == DividerPacketStrings[0])
                DividerPacketIndices[0] = Index;

            if (s == DividerPacketStrings[1])
                DividerPacketIndices[1] = Index;

            ++Index;

            std::cout << Packet.ToString() << std::endl;
        }

        std::cout << "Divider key: " << DividerPacketIndices[0] * DividerPacketIndices[1] << std::endl;

    #if 0
        size_t SumOfRightOrder = 0;
        size_t PairIndex = 1;
        std::ifstream fstream("Day13.txt"); 
        for(;!fstream.eof();)
        {
            ListNode first;
            ListNode second;

            std::string line;
            std::getline(fstream, line);
            {
                std::istringstream sstream(line);
                ReadNode(sstream, &first);
            }
            std::getline(fstream, line);
            {
                std::istringstream sstream(line);
                ReadNode(sstream, &second);
            }
            std::getline(fstream, line);

            int Compare = first.Compare(second);

            if (Compare <= 0)
            {
                std::cout << "  Right order" << std::endl;
                SumOfRightOrder += PairIndex;
            }
            else
            {
                std::cout << "  Wrong order" << std::endl;
            }
            PairIndex++;
        }

        std::cout << "Sum of right order: " << SumOfRightOrder << std::endl;
    #endif
    }
};

DECLARE_SOLUTION(CSolution13, 13, "Distress Signal");
