#ifndef __LSH_HASHSET_HPP__
#define __LSH_HASHSET_HPP__

#include <functional>
#include <limits>

#include "../Containers/Array.hpp"
#include "../Containers/StaticHashMap.hpp"
#include "../MetricSpace/Generic/HashFunction.hpp"
#include "../MetricSpace/Generic/DistanceFunction.hpp"
#include "../MetricSpace/Generic/DataSet.hpp"

namespace lsh
{
    template<typename PointType>
    class HashSet
    {
    public:
        using Point = typename PointType::Type;
        using PointRef = typename PointType::RefType;

        using DataSet = MetricSpace::Generic::DataSet<PointType>;
        using HashFunction = MetricSpace::Generic::HashFunction<PointType>;
        using DistanceFunction = MetricSpace::Generic::DistanceFunction<PointType>;

    private:
        const HashFunction& mHashFunc;
        const DistanceFunction& mDistFunc;
        Array<StaticHashMap<unsigned int>> mHashMapArray; /* TODO: Upgrade to a more cache friendly data structure */
        const DataSet& mDataSet;

    public:
        HashSet (const HashFunction& hashFunc, const DistanceFunction& distFunc, unsigned int hashMapSize, const DataSet& dataSet)
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

        void add (const DataSet& data)
        {
            for (unsigned int i = 0; i < data.getPointNum(); ++i)
            {
                add(i, data[i]);
            }
        }

        unsigned int forEachPointInRange (double R, const PointRef p, std::function<void (unsigned int, double)> func)
        {
            auto keySet = mHashFunc(p);
            unsigned int sum = 0;
            bool checked[mDataSet.getPointNum()] = { false };

            for (unsigned int i = 0; i < mHashMapArray.getLength(); ++i)
            {
                auto key = keySet[i];
                for (auto& x : mHashMapArray[i][key])
                {
                    //std::cout << x.target << std::endl;
                    if (!checked[x.target])
                    {
                        ++sum;
                        double dist = mDistFunc(mDataSet[x.target], p);
                        if (dist < R)
                        {
                            func(x.target, dist);
                            //sum++;
                        }
                        checked[x.target] = true;
                    }
                }
            }

            return sum;
        }

        unsigned int operator[] (const PointRef p);
    };
}

#endif /* end of include guard: __LSH_HASHSET_HPP__ */
