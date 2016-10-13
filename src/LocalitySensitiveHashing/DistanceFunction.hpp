#ifndef __LSH_DISTANCEFUNCTION_HPP__
#define __LSH_DISTANCEFUNCTION_HPP__

namespace lsh
{
    template<typename PointType, typename PointTypeRef = PointType&>
    struct DistanceFunction
    {
        using Point = PointType;
        using PointRef = PointTypeRef;
        
        virtual double operator() (const PointTypeRef x, const PointTypeRef y) const = 0;
    };
}

#endif /* end of include guard: __LSH_DISTANCEFUNCTION_HPP__ */
