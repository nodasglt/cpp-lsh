#ifndef __METRICSPACE_EUCLIDEAN_L2_DISTANCEFUNCTION_HPP__
#define __METRICSPACE_EUCLIDEAN_L2_DISTANCEFUNCTION_HPP__

#include "../Generic/DistanceFunction.hpp"
#include "Euclidean.hpp"

namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        struct DistanceFunction : public Generic::DistanceFunction<DataPoint>
        {
            double operator() (ConstPointRef x, ConstPointRef y) const override;
        };
    }
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_L2_DISTANCEFUNCTION_HPP__ */
