#ifndef __METRICSPACE_EUCLIDEAN_HPP__
#define __METRICSPACE_EUCLIDEAN_HPP__

#include "../../Containers/Block.hpp"

namespace MetricSpace {
namespace Euclidean
{
    struct DataPoint
    {
        using Type = Block<double*, double>;
        using RefType = Block<double*, double>;
    };
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_HPP__ */
