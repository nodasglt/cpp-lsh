#ifndef __METRICSPACE_EUCLIDEAN_COS_HASHFUNCTION_HPP__
#define __METRICSPACE_EUCLIDEAN_COS_HASHFUNCTION_HPP__

#include "Containers/Array.hpp"
#include "Containers/Matrix.hpp"
#include "../../Generic/HashFunction.hpp"
#include "../Euclidean.hpp"

namespace MetricSpace {
namespace Euclidean
{
    namespace Cos
    {
        struct HashFunction : public Generic::HashFunction<DataPoint>
        {
            HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, unsigned int vectorDim);

            uint64_t getKeyAtIndex (ConstPointRef p, unsigned int i) const override;

         private:
            Matrix<Array<double>> lines;
        };
    }
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_COS_HASHFUNCTION_HPP__ */
