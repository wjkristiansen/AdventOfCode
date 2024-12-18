#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <iomanip>

template<class _ValueType>
class Grid
{
public:
    using DataType = std::vector<std::vector<_ValueType>>;
    
protected:
    DataType m_Data;

public:
    Grid() = default;
    Grid(size_t width, size_t height, _ValueType initValue)
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

    void Print(char delim = '\0', int minSpacing = 0) const
    {
        for (const auto &row : m_Data)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                if (minSpacing > 0)
                    std::cout << std::setw(minSpacing);
                else
                    std::cout << row[i];
                if (delim != '\0' && i < row.size() - 1)
                    std::cout << delim;
            }
            std::cout << std::endl;
        }
    }
};

template<class _ValueType>
class DecimalDigitGrid : public Grid<_ValueType>
{
public:
    DecimalDigitGrid() = default;
    DecimalDigitGrid(size_t width, size_t height, _ValueType initValue) :
        Grid(width, height, initValue) {
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

            this->m_Data.push_back({});
            this->m_Data.back().reserve(line.length());
            std::transform(line.begin(), line.end(), std::back_inserter(this->m_Data.back()),
                [](char c)
                {
                    return c - '0';
                });
        }
    }
};

class AsciiGrid : public Grid<char>
{
public:
    AsciiGrid() = default;
    AsciiGrid(size_t width, size_t height, char initValue) :
        Grid(width, height, initValue) {}
    void ReadFromFile(const char* fileName)
    {
        std::ifstream fstream(fileName);
        while (!fstream.eof())
        {
            std::string line;
            std::getline(fstream, line);
            if (line.empty())
                break;

            this->m_Data.emplace_back(line.begin(), line.end());
        }
    }
};