#ifndef __METRICSPACE_DISTMATRIX_DISTANCEFUNCTION_HPP__
#define __METRICSPACE_DISTMATRIX_DISTANCEFUNCTION_HPP__

#include "../Generic/DistanceFunction.hpp"
#include "DistMatrix.hpp"

namespace MetricSpace
{
    namespace DistMatrix
    {
        struct DistanceFunction : public Generic::DistanceFunction<DataPoint>
        {
            double operator() (ConstPointRef x, ConstPointRef y) const override;
        };
    }
}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_DISTANCEFUNCTION_HPP__ */
