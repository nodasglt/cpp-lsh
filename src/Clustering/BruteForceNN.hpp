#ifndef __CLUSTERING_BRUTEFORCENN_HPP__
#define __CLUSTERING_BRUTEFORCENN_HPP__

#include "MetricSpace/Generic/Metric.hpp"

namespace Clustering
{
    template<typename PointType>
    class BruteForceNN
    {
    public:
        using Point = typename PointType::Type;
        using ConstPointRef = typename PointType::ConstRefType;

        using DataSet = MetricSpace::Generic::DataSet<PointType>;
        using HashFunction = MetricSpace::Generic::HashFunction<PointType>;
        using DistanceFunction = MetricSpace::Generic::DistanceFunction<PointType>;

        struct QueryResult
        {
            unsigned index;
            double dist;
        };

    private:
        const DistanceFunction& mDistFunc;
        const DataSet& mDataSet;

    public:
        BruteForceNN (const DistanceFunction& distFunc, const DataSet& dataSet)
            : mDistFunc(distFunc), mDataSet(dataSet) {}

        QueryResult operator() (ConstPointRef p) const
        {
            double dist = 0;
            unsigned index = 0;

            bool found = false;

            for (unsigned i = 0; i < mDataSet.getPointNum(); ++i)
            {
                double curDist = mDistFunc(p, mDataSet[i]);
                if ((curDist < dist || !found) && curDist > 0.0f)
                {
                    found = true;
                    dist = curDist;
                    index = i;
                }
            }

            return { index, dist };
        }
    };
}

#endif /* end of include guard: __CLUSTERING_BRUTEFORCENN_HPP__ */
