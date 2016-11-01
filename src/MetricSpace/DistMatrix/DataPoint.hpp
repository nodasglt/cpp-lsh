#ifndef __METRICSPACE_DISTMATRIX_HPP__
#define __METRICSPACE_DISTMATRIX_HPP__

#include <Containers/Block.hpp>

namespace MetricSpace {
namespace DistMatrix
{
    struct DataPoint
    {
        enum class Flag { data, test };

        //indicates whether it should be treated as a data point or a query point
        Flag flag;

        //its index in the data/test set
        unsigned id;

        //reference to the data/test set row containing the distances to all the other points in the data set
        Block<double> values;

        using Type = DataPoint;
        using ConstRefType = DataPoint;
    };
}}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_HPP__ */
