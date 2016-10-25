#ifndef __METRICSPACE_GENERIC_DISTANCEFUNCTION_HPP__
#define __METRICSPACE_GENERIC_DISTANCEFUNCTION_HPP__

namespace MetricSpace {
namespace Generic
{
    template<typename PointType>
    struct DistanceFunction
    {
        using Point = typename PointType::Type;
        using ConstPointRef = typename PointType::ConstRefType;

        virtual double operator() (ConstPointRef x, ConstPointRef y) const = 0;

        virtual ~DistanceFunction() {}
    };
}}

#endif /* end of include guard: __METRICSPACE_GENERIC_DISTANCEFUNCTION_HPP__ */
