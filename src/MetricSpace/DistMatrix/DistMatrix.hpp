#ifndef __METRICSPACE_DISTMATRIX_HPP__
#define __METRICSPACE_DISTMATRIX_HPP__

#include <Containers/Block.hpp>

namespace MetricSpace {
namespace DistMatrix
{
    struct DataPoint
    {
        enum class Flag { data, test };

        Flag flag;
        unsigned id;
        Block<double> values;

        using Type = DataPoint;
        using ConstRefType = DataPoint;
    };
}}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_HPP__ */
