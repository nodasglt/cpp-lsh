#ifndef __LSH_HASHSET_HPP__
#define __LSH_HASHSET_HPP__

#include <functional>
#include <limits>

#include "HashFunction.hpp"
#include "DistanceFunction.hpp"
#include "../Containers/Array.hpp"
#include "../Containers/StaticHashMap.hpp"
#include "../MetricSpace/Generic/DataSet.hpp"

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
        Array<StaticHashMap<unsigned int>> mHashMapArray;
        const MetricSpace::Generic::DataSet<PointType>& mDataSet;
        /* TODO: Store DataPoints to a list */

    public:
        HashSet (const HashFunction<PointType>& hashFunc, const DistanceFunction<PointType>& distFunc, unsigned int hashMapSize, const MetricSpace::Generic::DataSet<PointType>& dataSet)
            : mHashFunc(hashFunc), mDistFunc(distFunc), mHashMapArray(), mDataSet(dataSet)
        {
            mHashMapArray.reserve(mHashFunc.getKeyNum());

            for (unsigned int i = 0; i < mHashFunc.getKeyNum(); ++i)
            {
                mHashMapArray.emplaceBack(hashMapSize);
            }
        }

        void add (unsigned int value, const PointRef x) /* NOTE: can be removed */
        {
            auto keySet = mHashFunc(x);

            for (unsigned int i = 0; i < mHashMapArray.getLength(); ++i)
            {
                mHashMapArray[i].add(keySet[i], value);
            }
        }

        void add (const MetricSpace::Generic::DataSet<PointType>& data)
        {
            for (unsigned int i = 0; i < data.getPointNum(); ++i)
            {
                add(i, data[i]);
            }

            unsigned int i = 0;
            for (auto& x : mHashMapArray[0][32])
            {
                std::cout << i++ << " " << x.key << " " << x.target << std::endl;
            }
        }

        unsigned int forEachPointInRange (double R, const PointRef p, std::function<void (unsigned int)> func)
        {
            auto keySet = mHashFunc(p);
            unsigned int sum = 0;
            Array<unsigned int> checked;

            for (unsigned int i = 0; i < mHashMapArray.getLength(); ++i)
            {
                auto key = keySet[i];
                for (auto& x : mHashMapArray[i][key])
                {
                    if (key == x.key && !checked.contains(x.target))
                    {
                        std::cout << "/* message */" << std::endl;
                        if (mDistFunc(mDataSet[x.target], p) < R)
                        {
                            func(x.target);
                            checked.emplaceBack(x.target);
                            sum++;
                        }
                    }
                }
            }

            return sum;
        }

        unsigned int operator[] (const PointRef p);
    };
}

#endif /* end of include guard: __LSH_HASHSET_HPP__ */
