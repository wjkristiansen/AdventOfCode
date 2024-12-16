#include "pch.h"
#include "SolutionFactory.h"

class CSolution9: public CSolutionBase
{
public:
    CSolution9(): CSolutionBase()
    {
    }

    struct BlockRun
    {
        size_t Id;
        size_t Offset;
        size_t Length;

        BlockRun() = delete;
        BlockRun(size_t id, size_t offset, size_t length):
            Id(id),
            Offset(offset),
            Length(length)
        {}
    };

    std::list<BlockRun> m_BlockRuns;

    size_t ReadInput()
    {
        std::ifstream fstream("2024/Day9.txt");
        size_t position = 0;
        for(size_t fileId = 0; !fstream.eof(); ++fileId)
        {
            // Get the file size
            char fileSizeChar;
            fstream.read(&fileSizeChar, 1);
            size_t fileSize = fileSizeChar - '0';

            m_BlockRuns.emplace_back(fileId, position, static_cast<size_t>(fileSize));

            // Get the gap size
            char gapSizeChar;
            fstream.read(&gapSizeChar, 1);
            size_t gapSize = gapSizeChar - '0';

            position += fileSize + gapSize;
        }

        return position;
    }

    void PrintDiskContents()
    {
        size_t position = 0;
        for (auto& blockRun : m_BlockRuns)
        {
            size_t numGaps = blockRun.Offset - position;

            // Print gaps
            for (auto i = 0; i < numGaps; ++i)
                std::cout << '.';

            position += numGaps;

            // Print file id
            for (auto i = 0; i < blockRun.Length; ++i)
                std::cout << blockRun.Id;

            position += blockRun.Length;
        }
        std::cout << std::endl;
    }

    void Defrag()
    {
        // Seek to the first gap block
        for (auto it = m_BlockRuns.begin(); it != m_BlockRuns.end();)
        {
            auto itNext = std::next(it);
            if (itNext == m_BlockRuns.end())
                break;
            
            BlockRun& blockRun = *it;
            BlockRun& blockRunNext = *itNext;
            if (blockRunNext.Offset > blockRun.Offset + blockRun.Length)
            {
                // Move a block from the end into the gap
                BlockRun& back = m_BlockRuns.back();
                if (back.Id == it->Id)
                {
                    // Add to the current block
                    it->Length++;
                }
                else
                {
                    it = m_BlockRuns.insert(itNext, BlockRun(back.Id, blockRun.Offset + blockRun.Length, 1));
                }
                back.Length -= 1;
                if (back.Length == 0)
                {
                    // Remove the back run
                    m_BlockRuns.pop_back();
                }
                
                // PrintDiskContents();
            }
            else
            {
                it = itNext;
            }
        }
    }

    size_t Checksum()
    {
        size_t checksum = 0;
        for (auto& blockRun : m_BlockRuns)
        {
            for (size_t pos = blockRun.Offset; pos < blockRun.Offset + blockRun.Length; ++pos)
            {
                checksum += blockRun.Id * pos;
            }
        }
        return checksum;
    }

    virtual void Execute(int part)
    {
        if (part == 1)
        {
            ReadInput();
            // PrintDiskContents();
            Defrag();
            size_t checksum = Checksum();
            std::cout << "Part 1: Checksum: " << checksum << std::endl;
        }
        else
        {
        }
    }
};

DECLARE_SOLUTION(CSolution9, 9, "Disk Fragmenter");
