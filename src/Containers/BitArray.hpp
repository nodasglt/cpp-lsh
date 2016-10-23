#ifndef __BITARRAY_HPP__
#define __BITARRAY_HPP__

#include <iostream>
#include <assert.h>

#include "BitReference.hpp"

template<std::size_t sSize>
class BitArray
{
public:
    BitArray () : mData() {}

    void set (std::size_t i)
    {
        mData[i / sNumBits] |=  (1U << (i % sNumBits));
    }

    void reset (std::size_t i)
    {
        mData[i / sNumBits] &= ~(1U << (i % sNumBits));
    }

    void reset ()
    {
        for (unsigned i = 0; i < sRequiredBytes; ++i)
        {
            mData[i] = 0;
        }
    }

    void flip (std::size_t i)
    {
        mData[i / sNumBits] ^=  (1U << (i % sNumBits));
    }

    int count () const
    {
        static constexpr uint8_t setBitsAtIndex[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
        // setBits:
        // Look-up Table. Contains the number of set bits of all possible 4 bit numbers
        // index | binary | set_bits
        //   0  --> 0000 --> 0
        //   3  --> 0011 --> 2
        //   6  --> 0110 --> 2

        int sum = 0;
        for (unsigned i = 0; i < sRequiredBytes; ++i)
        {
            sum += setBitsAtIndex[mData[i] & 0xF] + setBitsAtIndex[(mData[i] >> 4) & 0xF];
        }
        return sum;
    }

    std::size_t getLength () const
    {
        return sSize;
    }

    constexpr bool operator[] (std::size_t i) const
    {
        return ((mData[i / sNumBits] & (1U<<(i % sNumBits))) != 0);
    }

    BitReference operator[] (std::size_t i)
    {
        return BitReference(mData, i, sNumBits);
    }

    BitArray& operator^= (const BitArray& other)
    {
        for (unsigned i = 0; i < sRequiredBytes; ++i)
        {
            mData[i] ^= other.mData[i];
        }

        return *this;
    }

    explicit operator uint64_t () const
    {
        static_assert(sSize <= 64, "BitArray: Length exceeds 64 bits. Cannot convert to uint64_t.");

        uint64_t rtn = 0;

        uint8_t shift = 0;

        for (unsigned i = 0; i < sRequiredBytes; ++i, shift += 8)
        {
            rtn |= (uint64_t)mData[i] << shift;
        }

        return rtn;
    }

private:
    static constexpr std::size_t sNumBits = 8 * sizeof(uint8_t);
    static constexpr std::size_t sRequiredBytes = (sSize + (sNumBits - 1)) / sNumBits;

    uint8_t mData[sRequiredBytes];
};

template<std::size_t sNumBits>
BitArray<sNumBits> operator^ (BitArray<sNumBits> x, const BitArray<sNumBits>& y)
{
    return x ^= y;
}

template<std::size_t sNumBits>
std::ostream& operator<< (std::ostream &os, const BitArray<sNumBits>& arr)
{
    os << '\"';

    for (unsigned i = 0; i < sNumBits; ++i)
    {
        os << arr[i];
    }

    return os << '\"';
}

#endif /* end of include guard: __BITARRAY_HPP__ */
