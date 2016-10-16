#ifndef __LSH_HASHSET_HPP__
#define __LSH_HASHSET_HPP__

#include <functional>

#include "Containers/Array.hpp"
#include "Containers/StaticHashMap.hpp"
#include "MetricSpace/Generic/Metric.hpp"

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

        struct QueryResult
        {
            bool found;
            unsigned int index;
            unsigned int sum;
        };

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

            for (unsigned int i = 0; i < mDataSet.getPointNum(); ++i)
            {
                auto keySet = mHashFunc(mDataSet[i]);

                for (unsigned int j = 0; j < mHashMapArray.getLength(); ++j)
                {
                    mHashMapArray[j].add(keySet[j], i);
                }
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
                    if (x.key == key && !checked[x.target])
                    {
                        sum++;
                        double dist = mDistFunc(mDataSet[x.target], p);
                        if (dist < R)
                        {
                            func(x.target, dist);
                        }
                        checked[x.target] = true;
                    }
                }
            }

            return sum;
        }

        QueryResult operator[] (const PointRef p)
        {
            auto keySet = mHashFunc(p);
            bool checked[mDataSet.getPointNum()] = { false };

            bool found = false;
            double sDist = 0;
            unsigned int r = 0;
            unsigned int sum = 0;

            for (unsigned int i = 0; i < mHashMapArray.getLength(); ++i)
            {
                auto key = keySet[i];
                for (auto& x : mHashMapArray[i][key])
                {
                    if (x.key == key && !checked[x.target])
                    {
                        sum++;
                        double dist = mDistFunc(mDataSet[x.target], p);
                        if ((dist < sDist || !found) && dist > 0/* NOTE: ignore existing values for testing -- remove when testSet is available */)
                        {
                            found = true;
                            sDist = dist;
                            r = x.target;
                        }
                        checked[x.target] = true;
                    }
                }
            }

            return {found, r, sum};
        }
    };
}

/*

void add (unsigned int value, const PointRef x)
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

*/

#endif /* end of include guard: __LSH_HASHSET_HPP__ */
