#pragma once
#include <cstdint>

template<class PartType> struct PartTraits;

template<> struct PartTraits<uint32_t>
{
    using DoubleType = uint64_t;
};

template<> struct PartTraits<uint16_t>
{
    using DoubleType = uint32_t;
};

template<> struct PartTraits<uint8_t>
{
    using DoubleType = uint16_t;
};

template<class PartType>
class BigUInt
{
    using DoubleType = typename PartTraits<PartType>::DoubleType;
    static const PartType Mask = static_cast<PartType>(0 - 1);
    static const int Shift = 8 * sizeof(PartType);
    std::vector<PartType> m_parts;

    void Prune()
    { 
        // Strip leading zeros
        for (auto it = m_parts.end();; )
        {
            --it;

            if (it == m_parts.begin())
                break;

            if (*it != 0)
                break;

            it = m_parts.erase(it);
        }
    }

public:
    BigUInt()
    {
        m_parts.emplace_back(PartType(0U));
    }

    BigUInt(const PartType &n)
    {
        m_parts.emplace_back(n);
    }

    BigUInt(int n)
    {
        m_parts.emplace_back(PartType(n));
    }

    BigUInt(const DoubleType& n) :
        m_parts(2)
    {
        m_parts[0] = PartType(n);
        m_parts[1] = PartType(n >> Shift);
    }

    BigUInt(const BigUInt& o)
    {
        m_parts = o.m_parts;
    }

    size_t NumParts() const { return m_parts.size(); }
    PartType Part(int index) const { return index < NumParts() ? m_parts[index] : PartType(0U); }

    PartType AsPartType() const { return Part(0); }

    DoubleType AsDoubleType() const
    {
        DoubleType value = PartType();
        if (NumParts() > 1)
        {
            value += DoubleType(Part(1)) << Shift;
        }

        return value;
    }

    BigUInt operator*(const BigUInt& o) const
    {
        BigUInt result;
        result.m_parts.resize(o.NumParts() + NumParts());

        PartType carry = 0;

        for (size_t j = 0; j < o.NumParts(); ++j)
        {
            auto& APart = o.m_parts[j];

            for (size_t i = 0; i < NumParts(); ++i)
            {
                auto& BPart = m_parts[i];
                DoubleType PartProduct = carry + APart * BPart;
                result.m_parts[i + j] += static_cast<PartType>(PartProduct);
                carry = static_cast<PartType>(PartProduct >> Shift);
            }
        }

        if (carry)
            result.m_parts.back() = carry;

        result.Prune();

        return result;
    }

    BigUInt operator+(const BigUInt& o) const
    {
        BigUInt result;
        result.m_parts.resize(std::max(o.NumParts(), NumParts()));
        PartType carry(0U);

        for (int i = 0; i < result.NumParts(); ++i)
        {
            std::size_t sum = this->Part(i);
            sum += o.Part(i);
            sum += carry;
            result.m_parts[i] = static_cast<PartType>(sum);
            carry = static_cast<PartType>(sum >> Shift);
        }

        result.Prune();

        return result;
    }

    BigUInt operator/(const DoubleType &dividend) const
    {
        BigUInt result;

        DoubleType chunk = 0;
        result.m_parts.resize(NumParts());

        for(size_t index = NumParts(); index != 0;)
        {
            --index;

            chunk += m_parts[index];

            if (chunk > dividend)
            {
                PartType quotient = PartType(chunk / dividend);
                DoubleType remainder = chunk - (dividend * quotient);
                result.m_parts[index] = quotient;
                chunk = remainder << Shift;
            }
            else
            {
                result.m_parts[index] = 0;
                chunk = chunk << Shift;
            }
        }

        result.Prune();

        return result;
    }

    BigUInt operator/(const BigUInt& o) const
    {
        // Maybe too trick for the current challenge.
        // Assume o is convertible to PartType
        return operator/(o.AsDoubleType());
    }

    bool operator==(const BigUInt& o) const
    {
        if(NumParts() != o.NumParts())
            return false;

        for (size_t i = 0; i < NumParts(); ++i)
        {
            if(m_parts[i] != o.m_parts[i])
                return false;
        }

        return true;
    }

    bool operator<(const BigUInt& o) const
    {
        if (NumParts() < o.NumParts())
            return true;

        if (NumParts() > o.NumParts())
            return false;

        for (size_t i = NumParts(); i > 0;)
        {
            --i;
            if (m_parts[i] < o.m_parts[i])
                return true;

            if (m_parts[i] > o.m_parts[i])
                return false;
        }

        return false;
    }

    bool operator>(const BigUInt& o) const
    {
        if (NumParts() > o.NumParts())
            return true;

        if (NumParts() < o.NumParts())
            return false;

        for (size_t i = NumParts(); i > 0;)
        {
            --i;
            if (m_parts[i] > o.m_parts[i])
                return true;

            if (m_parts[i] < o.m_parts[i])
                return false;
        }

        return false;
    }

    bool operator<=(const BigUInt& o) const
    {
        if (NumParts() < o.NumParts())
            return true;

        if (NumParts() > o.NumParts())
            return false;

        for (size_t i = NumParts(); i > 0;)
        {
            --i;
            if (m_parts[i] < o.m_parts[i])
                return true;

            if (m_parts[i] > o.m_parts[i])
                return false;
        }

        return true;
    }

    bool operator>=(const BigUInt& o) const
    {
        if (NumParts() > o.NumParts())
            return true;

        if (NumParts() < o.NumParts())
            return false;

        for (size_t i = NumParts(); i > 0;)
        {
            --i;
            if (m_parts[i] > o.m_parts[i])
                return true;

            if (m_parts[i] < o.m_parts[i])
                return false;
        }

        return true;
    }
};
