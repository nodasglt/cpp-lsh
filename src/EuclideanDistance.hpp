#ifndef __EUCLIDEANDISTANCE_HPP__
#define __EUCLIDEANDISTANCE_HPP__

#include "LocalitySensitiveHashing/DistanceFunction.hpp"
#include "VectorSet.hpp"

namespace L2
{
    struct DistanceFunction : public lsh::DistanceFunction<VectorSet::Point, VectorSet::PointRef>
    {
        double operator() (const PointRef x, const PointRef y) const;
    };
}

#endif /* end of include guard: __EUCLIDEANDISTANCE_HPP__ */
