#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <exception>
#include <assert.h>

#include "..\SolutionFactory.h"

class Node
{
public:
    Node() = delete;
    Node(Node *pParent) :
        m_pParent(pParent) {}
    virtual ~Node() = default;

    virtual bool IsDirectory() const = 0;
    virtual size_t GetSize() const = 0;

    virtual Node *GetParent() { return m_pParent; }
    virtual Node *GetChild(const std::string &) { return nullptr; }
    virtual Node *AddChildDir(const std::string &) { return nullptr; }
    virtual Node *AddChildFile(const std::string &, size_t ) { return nullptr; }
    virtual size_t SumOfDirs(size_t) const { return 0; }

    Node *m_pParent = nullptr;
};

class FileNode : public Node
{
public:
    FileNode(class Node *pParent, size_t Size) :
        m_Size(Size),
        Node(pParent) {}

    virtual bool IsDirectory() const { return false; }
    virtual size_t GetSize() const { return m_Size; }

public:
    size_t m_Size;
};

class DirectoryNode : public Node
{
public:
    DirectoryNode(class Node *pParent) :
        Node(pParent) {}
    virtual bool IsDirectory() const { return true; }

    virtual size_t GetSize() const
    {
        size_t Size = 0;
        for(auto &e : m_ChildNodes)
        {
            Size += e.second->GetSize();
        }

        return Size;
    }

    virtual size_t SumOfDirs(size_t MaxSize) const
    {
        size_t Size = 0;
        size_t LocalSize = GetSize();
        if (LocalSize <= MaxSize)
            Size += LocalSize;

        for (auto& e : m_ChildNodes)
        {
            if (e.second->IsDirectory())
            {
                Size += e.second->SumOfDirs(MaxSize);
            }
        }

        return Size;
    }

    virtual Node *GetChild(const std::string &Name)
    {
        auto it = m_ChildNodes.find(Name);
        if(it != m_ChildNodes.end())
        {
            return it->second.get();
        }

        return nullptr;
    }

    template <class ChildType, typename ...Args>
    Node *AddChild(const std::string &Name, Args... args)
    {
        auto [it, success] = m_ChildNodes.emplace(Name, std::make_unique<ChildType>(this, args...));
        if(!success)
            return nullptr;
        
        return it->second.get();
    }

    virtual Node *AddChildDir(const std::string &Name)
    {
        return AddChild<DirectoryNode>(Name);
    }

    virtual Node *AddChildFile(const std::string &Name, size_t Size)
    {
        return AddChild<FileNode>(Name, Size);
    }


public:
    std::map<std::string, std::unique_ptr<Node>> m_ChildNodes;
};

void CSolution<2022, 7>::Execute()
{
    DirectoryNode root(nullptr);
    std::ifstream fstream("Day7.txt");
    Node *pCurDir = &root;

    for(; !fstream.eof();)
    {
        std::string token;
        fstream >> token;

        for(;!fstream.eof();)
        {

            if (token.empty())
                break;

            assert(token == std::string("$"));

            // Fetch command
            fstream >> token;

            if(token == std::string("cd"))
            {
                fstream >> token;

                if(token == "..")
                {
                    // Move out one level
                    pCurDir = pCurDir->GetParent();
                }
                else if (token == "/")
                {
                    // Move to the root
                    pCurDir = &root;
                }
                else
                {
                    // Move into child directory
                    pCurDir = pCurDir->GetChild(token);
                    assert(pCurDir);
                }
                fstream >> token;
            }
            else if( token == std::string("ls"))
            {
                for(;;)
                {
                    fstream >> token;
                    if(fstream.eof() || token == "$" || token.empty())
                        break;

                    if(token == "dir")
                    {
                        fstream >> token;
                        pCurDir->AddChildDir(token);
                    }
                    else
                    {
                        size_t Size = std::stoull(token);
                        fstream >> token;
                        pCurDir->AddChildFile(token, Size);
                    }
                }
            }
        }
    }

    // Find all directories with size <= 100000 and accumulate the size

    std::cout << root.SumOfDirs(100000) << std::endl;
}