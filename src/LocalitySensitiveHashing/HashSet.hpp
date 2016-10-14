#ifndef __LSH_HASHSET_HPP__
#define __LSH_HASHSET_HPP__

#include <functional>

#include "HashFunction.hpp"
#include "DistanceFunction.hpp"
#include "../Containers/Array.hpp"
#include "../Containers/StaticHashMap.hpp"

namespace lsh
{
    template<typename PointType>
    class HashSet
    {
    public:
        using Point = typename PointType::Type;
        using PointRef = typename PointType::RefType;

    private:
        const HashFunction<PointType>& mHashFunc;
        const DistanceFunction<PointType>& mDistFunc;
        Array<StaticHashMap</*uint64_t, */Point>> mHashMapArray;
        /* TODO: Store DataPoints to a list */

    public:
        HashSet (const HashFunction<PointType>& hashFunc, const DistanceFunction<PointType>& distFunc, unsigned int hashMapSize) : mHashFunc(hashFunc), mDistFunc(distFunc), mHashMapArray()
        {
            mHashMapArray.reserve(mHashFunc.getKeyNum());

            for (unsigned int i = 0; i < mHashFunc.getKeyNum(); ++i)
            {
                mHashMapArray.emplaceBack(hashMapSize);
            }
        }

        void add (const PointRef x) /* NOTE: can be removed */
        {
            auto keySet = mHashFunc(x);

            for (auto i = mHashMapArray.getLength(); i >= 0; --i)
            {
                mHashMapArray[i].add(keySet[i], x);
            }
        }

        unsigned int forEachNNinRange (unsigned int r, std::function<void (const PointRef)> func);

        unsigned int operator[] (const PointRef p);
    };
}

#endif /* end of include guard: __LSH_HASHSET_HPP__ */
