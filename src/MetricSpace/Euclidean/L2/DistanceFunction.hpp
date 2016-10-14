#ifndef __METRICSPACE_EUCLIDEAN_L2_DISTANCEFUNCTION_HPP__
#define __METRICSPACE_EUCLIDEAN_L2_DISTANCEFUNCTION_HPP__

#include "../../../LocalitySensitiveHashing/DistanceFunction.hpp"
#include "../Euclidean.hpp"

namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        struct DistanceFunction : public lsh::DistanceFunction<DataPoint>
        {
            double operator() (const PointRef x, const PointRef y) const;
        };
    }
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_L2_DISTANCEFUNCTION_HPP__ */
