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
        class HashFunction : public Generic::HashFunction<DataPoint>
        {
        public:
            HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, const DistanceFunction* dist);

            uint64_t getKeyAtIndex (ConstPointRef p, unsigned int i) const override;

            // After we process the dataSet we switch to a different distance Matrix for the testSet.
            // When an actual distance function can be provided this will not be required.
            void setDistFunction (const DistanceFunction* dist);

         private:
            struct line
            {
                ConstPointRef x, y;
                double length;
                double midValue;
            };

            const DistanceFunction* mDistFunc;
            Matrix<line> mLines;

            double project (line l, ConstPointRef indexToProject) const;
        };
    }
}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_HASHFUNCTION_HPP__ */
