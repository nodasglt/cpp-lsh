#ifndef __LSH_DISTANCEFUNCTION_HPP__
#define __LSH_DISTANCEFUNCTION_HPP__

namespace MetricSpace {
namespace Generic
{
    template<typename PointType>
    struct DistanceFunction
    {
        using Point = typename PointType::Type;
        using PointRef = typename PointType::RefType;

        virtual double operator() (const PointRef x, const PointRef y) const = 0;
    };
}}

#endif /* end of include guard: __LSH_DISTANCEFUNCTION_HPP__ */
