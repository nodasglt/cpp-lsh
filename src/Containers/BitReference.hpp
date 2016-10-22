#ifndef __BITREFERENCE_HPP__
#define __BITREFERENCE_HPP__

class BitReference
{
public:
    BitReference(uint8_t* byteArray, std::size_t i, std::size_t numBits)
    : mByte(byteArray[i / numBits]), mMask(1U << (i % numBits)) {}
    
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

private:
    uint8_t& mByte;
    uint8_t  mMask;
};

#endif /* end of include guard: __BITREFERENCE_HPP__ */
