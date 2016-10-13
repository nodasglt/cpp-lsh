#ifndef __EUCLIDEANHASH_HPP__
#define __EUCLIDEANHASH_HPP__

#include "Containers/Array.hpp"
#include "Containers/Matrix.hpp"
#include "LocalitySensitiveHashing/HashFunction.hpp"
#include "VectorSet.hpp"

namespace L2
{
    struct HashFunction : public lsh::HashFunction<VectorSet::Point, VectorSet::PointRef>
    {
        HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, unsigned int vectorDim, double window);

        uint64_t getKeyAtIndex (const VectorSet::Point p, unsigned int i) const;

     private:
        Matrix<Array<double>> lines;
        Matrix<double> constants;
        const double window;
    };
}

#endif /* end of include guard: __EUCLIDEANHASH_HPP__ */
