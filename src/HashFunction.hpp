#ifndef __HASHFUNCTION_HPP__
#define __HASHFUNCTION_HPP__

#include "Containers/Matrix.hpp"
#include <cstdlib>

namespace lsh
{
    template<typename PointType>
    struct HashFunction
    {
        struct KeyGenerator
        {
            const HashFunction& mHashData;
            const PointType& mPoint;

            class iterator
            {
             private:
                 unsigned int mIndex;
                 const KeyGenerator& mKeyGen;

             public:
                 iterator (unsigned int index, const KeyGenerator& keyGen) : mIndex(index), mKeyGen(keyGen) {}

                 bool operator!= (iterator other) const
                 {
                     return (mIndex != other.mIndex);
                 }

                 void operator++ ()
                 {
                     mIndex++;
                 }

                 uint64_t operator* () const
                 {
                     return mKeyGen.mHashData.getKeyAtIndex(mKeyGen.mPoint, mIndex);
                 }
            };

            KeyGenerator (const HashFunction& hashData, const PointType& dataPoint) : mHashData(hashData), mPoint(dataPoint) {}

            uint64_t operator[] (unsigned int i) const
            {
                return mHashData.getKeyAtIndex(mPoint, i);
            }

            iterator begin()
            {
                return {0, *this};
            }

            const iterator begin() const
            {
                return {0, *this};
            }

            iterator end()
            {
                return {mHashData.getKeyNum(), *this};
            }

            const iterator end() const
            {
                return {mHashData.getKeyNum(), *this};
            }
        };

        explicit HashFunction (unsigned int keyNum) : mKeyNum(keyNum) {}

        unsigned int getKeyNum() const
        {
            return mKeyNum;
        }

        const KeyGenerator operator() (const PointType& p) const
        {
            return KeyGenerator(*this, p);
        }

        virtual uint64_t getKeyAtIndex (const PointType& p, unsigned int i) const = 0;

     private:
        const unsigned int mKeyNum;
    };
}

#endif /* end of include guard: __HASHFUNCTION_HPP__ */
