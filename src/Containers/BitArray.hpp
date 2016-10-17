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

            return *this;
        }

        BitReference& operator= (const BitReference& x)
        {
            mByte = x.mByte;
            mMask = x.mMask;

            return *this;
        }

        operator bool() const
        {
            return mByte & mMask;
        }

        bool operator~() const
        {
            return ~ bool{ *this };
        }

        BitReference& flip()
        {
            mByte ^= mMask;

            return *this;
        }

        friend class BitArray;

    private:
        BitReference(uint8_t* byteArray, std::size_t i) : mByte(byteArray[i / sNumBits]), mMask(1U << (i % sNumBits)) {}

        uint8_t& mByte;
        uint8_t  mMask;
    };

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

#endif /* end of include guard: __BITARRAY_HPP__ */
