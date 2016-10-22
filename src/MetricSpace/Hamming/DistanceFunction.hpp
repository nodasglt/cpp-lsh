#ifndef __METRICSPACE_HAMMING_DISTANCEFUNCTION_HPP__
#define __METRICSPACE_HAMMING_DISTANCEFUNCTION_HPP__

#include "../Generic/DistanceFunction.hpp"
#include "Hamming.hpp"

namespace MetricSpace
{
    namespace Hamming
    {
        struct DistanceFunction : public Generic::DistanceFunction<DataPoint>
        {
            double operator() (ConstPointRef x, ConstPointRef y) const override;
        };
    }
}

#endif /* end of include guard: __METRICSPACE_HAMMING_DISTANCEFUNCTION_HPP__ */
