#ifndef __LSH_HASHFUNCTION_HPP__
#define __LSH_HASHFUNCTION_HPP__

namespace lsh
{
    template<typename PointType>
    struct HashFunction
    {
        using Point = typename PointType::Type;
        using PointRef = typename PointType::RefType;

        struct KeyGenerator
        {
            const HashFunction& mHashData;
            const PointRef mPoint;

            KeyGenerator (const HashFunction& hashData, const PointRef dataPoint) : mHashData(hashData), mPoint(dataPoint) {}

            uint64_t operator[] (unsigned int i) const
            {
                return mHashData.getKeyAtIndex(mPoint, i);
            }
        };

        explicit HashFunction (unsigned int keyNum) : mKeyNum(keyNum) {}

        unsigned int getKeyNum() const
        {
            return mKeyNum;
        }

        const KeyGenerator operator() (const PointRef p) const
        {
            return KeyGenerator(*this, p);
        }

        virtual uint64_t getKeyAtIndex (const PointRef p, unsigned int i) const = 0;

     private:
        const unsigned int mKeyNum;
    };
}

#endif /* end of include guard: __LSH_HASHFUNCTION_HPP__ */
