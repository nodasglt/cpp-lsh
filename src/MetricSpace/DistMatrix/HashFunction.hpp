#ifndef __METRICSPACE_DISTMATRIX_HASHFUNCTION_HPP__
#define __METRICSPACE_DISTMATRIX_HASHFUNCTION_HPP__

#include "Containers/Matrix.hpp"
#include "../Generic/HashFunction.hpp"
#include "DistanceFunction.hpp"
#include "DistMatrix.hpp"

namespace MetricSpace
{
    namespace DistMatrix
    {
        struct HashFunction : public Generic::HashFunction<DataPoint>
        {
            HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, const DistanceFunction& dist);

            uint64_t getKeyAtIndex (ConstPointRef p, unsigned int i) const override;

         private:
            struct line
            {
                ConstPointRef x, y;
                double midValue;
            };

            const DistanceFunction& distFunc;
            Matrix<line> lines;

            double project (ConstPointRef x, ConstPointRef y, ConstPointRef indexToProject) const;
            double project (line l, ConstPointRef indexToProject) const;
        };
    }
}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_HASHFUNCTION_HPP__ */
