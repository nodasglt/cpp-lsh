#ifndef __DISTANCEFUNCTION_HPP__
#define __DISTANCEFUNCTION_HPP__

namespace lsh
{
    template<typename PointType>
    struct DistanceFunction
    {
        virtual double operator() (const PointType& x, const PointType& y) const = 0;
    };
}

#endif /* end of include guard: __DISTANCEFUNCTION_HPP__ */
