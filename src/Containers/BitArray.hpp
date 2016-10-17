#ifndef __BITARRAY_HPP__
#define __BITARRAY_HPP__

#include <iostream>
#include <assert.h>

template<std::size_t sSize>
class BitArray
{
public:
    class BitReference
    {
    public:
        BitReference& operator= (bool x)
        {
            mByte = (x) ? (mByte | mMask) : (mByte & ~mMask);
        }

        BitReference& operator= (const BitReference& x)
        {
            mByte = x.mByte;
            mMask = x.mMask;
        }

        operator bool() const
        {
            return bool{ mByte & mMask };
        }

        bool operator~() const
        {
            return ~ bool{ *this };
        }

        BitReference& flip()
        {
            mByte ^= mMask;
        }

    private:
        BitReference(uint8_t* byteArray, std::size_t i) : mByte(byteArray[i / sNumBits]), mMask(1U << (i % sNumBits)) {}

        uint8_t& mByte;
        uint8_t  mMask;
    };

    friend class BitReference;

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

    void flip (std::size_t i)
    {
        mData[i / sNumBits] ^=  (1U << (i % sNumBits));
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
        return BitReference(mData, i);
    }

private:
    static constexpr std::size_t sNumBits = sizeof(uint8_t);
    static constexpr std::size_t sRequiredBytes = (sSize + (sNumBits - 1)) / sNumBits;

    uint8_t mData[sRequiredBytes];
};

#endif /* end of include guard: __BITARRAY_HPP__ */
