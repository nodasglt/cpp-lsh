#ifndef __LSH_HASHSET_HPP__
#define __LSH_HASHSET_HPP__

#include <functional>

#include "Containers/Array.hpp"
#include "Containers/StaticHashMap.hpp"
#include "MetricSpace/Generic/Metric.hpp"
#include "Containers/DynamicBitArray.hpp"

namespace lsh
{
    template<typename PointType>
    class HashSet
    {
    public:
        using Point = typename PointType::Type;
        using ConstPointRef = typename PointType::ConstRefType;

        using DataSet = MetricSpace::Generic::DataSet<PointType>;
        using HashFunction = MetricSpace::Generic::HashFunction<PointType>;
        using DistanceFunction = MetricSpace::Generic::DistanceFunction<PointType>;

        struct QueryResult
        {
            bool found;
            unsigned index;
            unsigned sum;
        };

    private:
        const HashFunction& mHashFunc;
        const DistanceFunction& mDistFunc;
        Array<StaticHashMap<uint64_t, Array<unsigned>>> mHashMapArray; /* TODO: Upgrade to a more cache friendly data structure */
        const DataSet& mDataSet;

    public:
        HashSet (const HashFunction& hashFunc, const DistanceFunction& distFunc, unsigned hashMapSize, const DataSet& dataSet)
            : mHashFunc(hashFunc), mDistFunc(distFunc), mHashMapArray(), mDataSet(dataSet)
        {
            mHashMapArray.reserve(mHashFunc.getKeyNum());

            for (unsigned i = 0; i < mHashFunc.getKeyNum(); ++i)
            {
                mHashMapArray.emplaceBack(hashMapSize);
            }

            for (unsigned i = 0; i < mDataSet.getPointNum(); ++i)
            {
                auto keySet = mHashFunc(mDataSet[i]);

                for (unsigned j = 0; j < mHashMapArray.getLength(); ++j)
                {
                    auto key = keySet[j];
                    if (mHashMapArray[j].exists(key))
                    {
                        mHashMapArray[j][key].emplaceBack(i);
                    }
                    else
                    {
                        mHashMapArray[j].add(key, Array<unsigned>(1, i));
                    }
                }
            }
            //std::cout << mHashMapArray << std::endl;

            int i = 0;
            mHashMapArray[0].for_each([&](auto& x)
            {
                std::cout << '[' << i++ << "] " << x.key << " : " << x.target << std::endl;
            });
        }

/*
            // NOTE: Experimental thread support
            Array<std::thread> pool;
            pool.reserve(mHashMapArray.getLength());

            for (unsigned int j = 0; j < mHashMapArray.getLength(); ++j)
            {
                pool.emplaceBack(std::thread([this, j]()
                {
                    for (unsigned int i = 0; i < mDataSet.getPointNum(); ++i)
                    {
                        auto key = mHashFunc.getKeyAtIndex(mDataSet[i], j);
                        mHashMapArray[j].add(key, i);
                    }
                }));
            }

            for (auto& x : pool)
            {
                x.join();
            }
*/

        unsigned int forEachPointInRange (double R, ConstPointRef p, std::function<void (unsigned int, double)> func)
        {
            auto keySet = mHashFunc(p);
            unsigned sum = 0;

            Array<bool> checked(mDataSet.getLength(), false);

            for (unsigned i = 0; i < mHashMapArray.getLength(); ++i)
            {
                auto key = keySet[i];
                if (!mHashMapArray[i].exists(key)) continue;
                for (auto x : mHashMapArray[i][key])
                {
                    if (!checked[x])
                    {
                        sum++;
                        double dist = mDistFunc(mDataSet[x], p);
                        if (dist < R)
                        {
                            func(x, dist);
                        }
                        checked[x] = true;
                    }
                }
            }

            return sum;
        }

        QueryResult operator[] (ConstPointRef p)
        {
            auto keySet = mHashFunc(p);

            Array<bool> checked(mDataSet.getPointNum(), false);

            bool found = false;
            double sDist = 0;
            unsigned r = 0;
            unsigned sum = 0;

            for (unsigned i = 0; i < mHashMapArray.getLength(); ++i)
            {
                auto key = keySet[i];
                if (!mHashMapArray[i].exists(key)) continue;
                for (auto x : mHashMapArray[i][key])
                {
                    if (!checked[x])
                    {
                        sum++;
                        double dist = mDistFunc(mDataSet[x], p);
                        if ((dist < sDist || !found) && dist > 0/* NOTE: ignore existing values for testing -- remove when testSet is available */)
                        {
                            found = true;
                            sDist = dist;
                            r = x;
                        }
                        checked[x] = true;
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
