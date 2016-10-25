#ifndef __METRICSPACE_GENERIC_HASHFUNCTION_HPP__
#define __METRICSPACE_GENERIC_HASHFUNCTION_HPP__

#include <cinttypes>

namespace MetricSpace {
namespace Generic
{
    template<typename PointType>
    struct HashFunction
    {
        using Point = typename PointType::Type;
        using ConstPointRef = typename PointType::ConstRefType;

        struct KeyGenerator
        {
            const HashFunction& mHashData;
            ConstPointRef mPoint;

            KeyGenerator (const HashFunction& hashData, ConstPointRef dataPoint) : mHashData(hashData), mPoint(dataPoint) {}

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

        const KeyGenerator operator() (ConstPointRef p) const
        {
            return KeyGenerator(*this, p);
        }

        virtual uint64_t getKeyAtIndex (ConstPointRef p, unsigned int i) const = 0;

        virtual ~HashFunction() {}

     private:
        const unsigned int mKeyNum;
    };
}}

#endif /* end of include guard: __METRICSPACE_GENERIC_HASHFUNCTION_HPP__ */
