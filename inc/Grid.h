#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>

template<class _ValueType>
class DecimalGrid
{
public:
    using DataType = std::vector<std::vector<_ValueType>>;
    
private:
    DataType m_Data;

public:
    DecimalGrid() = default;
    DecimalGrid(size_t width, size_t height, _ValueType initValue)
    {
        std::vector<_ValueType> rowData(width, initValue);
        m_Data = DataType(height, rowData);
    }

    DataType& Data() { return m_Data; }
    const DataType& Data() const { return m_Data; }

    size_t Width() const { return m_Data.front().size(); }
    size_t Height() const { return m_Data.size(); }
    void SetValue(_ValueType v, size_t row, size_t col)
    {
        if (row >= Height() || col >= Width())
            throw(std::exception());
        m_Data[row][col] = v;
    }

    _ValueType GetValue(size_t row, size_t col) const
    {
        if (row >= Height() || col >= Width())
            throw(std::exception());
        return m_Data[row][col];
    }

    void ReadDenseSingleDigitsFromFile(const char* fileName)
    {
        std::ifstream fstream(fileName);
        while (!fstream.eof())
        {
            std::string line;
            std::getline(fstream, line);
            if (line.empty())
                break;
            
            m_Data.push_back({});
            m_Data.back().reserve(line.length());
            std::transform(line.begin(), line.end(), std::back_inserter(m_Data.back()),
                [](char c)
                {
                    return c - '0';
                });
        }
    }

    void Print() const
    {
        for (const auto &row : m_Data)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                std::cout << row[i];
                if (i < row.size() - 1)
                    std::cout << ",";
            }
            std::cout << std::endl;
        }
    }
};
