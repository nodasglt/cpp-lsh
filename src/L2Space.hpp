#ifndef __L2HASHFUNCTION_HPP__
#define __L2HASHFUNCTION_HPP__

#include "Containers/Array.hpp"
#include "HashFunction.hpp"
#include "DistanceFunction.hpp"

namespace L2
{
    using Point = Array<double>;

    struct HashFunction : public lsh::HashFunction<Point>
    {
        HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, unsigned int vectorDim, double window);

        uint64_t getKeyAtIndex (const Point& p, unsigned int i) const;

     private:
        Matrix<Point> lines;
        Matrix<double> constants;
        const double window;
    };

    struct DistanceFunction : public lsh::DistanceFunction<Point>
    {
        double operator() (const Point& x, const Point& y) const;
    };
}

#endif /* end of include guard: __L2HASHFUNCTION_HPP__ */
