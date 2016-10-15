#ifndef __METRICSPACE_EUCLIDEAN_L2_HASHFUNCTION_HPP__
#define __METRICSPACE_EUCLIDEAN_L2_HASHFUNCTION_HPP__

#include "../../../Containers/Array.hpp"
#include "../../../Containers/Matrix.hpp"
#include "../../Generic/HashFunction.hpp"
#include "../Euclidean.hpp"

namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        struct HashFunction : public Generic::HashFunction<DataPoint>
        {
            HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, unsigned int vectorDim, double window);

            uint32_t getKeyAtIndex (const PointRef p, unsigned int i) const;

         private:
            Matrix<Array<double>> lines;
            Matrix<double> constants;
            const double window;
        };
    }
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_L2_HASHFUNCTION_HPP__ */
