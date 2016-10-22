#ifndef __DYNAMICBITARRAY_HPP__
#define __DYNAMICBITARRAY_HPP__

#include <iostream>
#include <assert.h>

#include "BitReference.hpp"

class DynamicBitArray
{
public:
    DynamicBitArray (std::size_t size) : mSize(size), mData(requiredBytes(size), false) {}

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
        for (auto& x : mData)
        {
            x = 0;
        }
    }

    void flip (std::size_t i)
    {
        mData[i / sNumBits] ^=  (1U << (i % sNumBits));
    }

    std::size_t getLength () const
    {
        return mSize;
    }

    bool operator[] (std::size_t i) const
    {
        return ((mData[i / sNumBits] & (1U<<(i % sNumBits))) != 0);
    }

    BitReference operator[] (std::size_t i)
    {
        return BitReference(&mData[0], i, mSize);
    }

    explicit operator uint64_t () const
    {
        assert(mSize <= 64);

        uint64_t rtn = 0;

        uint8_t shift = 0;

        for (unsigned i = 0; i < mData.getLength(); ++i, shift += 8)
        {
            rtn |= (uint64_t)mData[i] << shift;
        }

        return rtn;
    }

private:
    static constexpr std::size_t sNumBits = 8 * sizeof(uint8_t);
    static inline std::size_t requiredBytes (std::size_t size)
    {
        return (size + (sNumBits - 1)) / sNumBits;
    }

    std::size_t mSize;
    Array<uint8_t> mData;
};

#endif /* end of include guard: __DYNAMICBITARRAY_HPP__ */
