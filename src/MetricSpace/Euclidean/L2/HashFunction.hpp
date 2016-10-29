#ifndef __METRICSPACE_EUCLIDEAN_L2_HASHFUNCTION_HPP__
#define __METRICSPACE_EUCLIDEAN_L2_HASHFUNCTION_HPP__

#include "Containers/Array.hpp"
#include "Containers/Matrix.hpp"
#include "../../Generic/HashFunction.hpp"
#include "../Euclidean.hpp"

namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        class HashFunction : public Generic::HashFunction<DataPoint>
        {
        public:
            HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, unsigned int vectorDim, double window);

            uint64_t getKeyAtIndex (ConstPointRef p, unsigned int i) const override;

         private:
            Matrix<Array<double>> mLines;
            Matrix<double> mConstants;
            Matrix<int> mHashFunctionWeights;
            const double mWindow;
        };
    }
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_L2_HASHFUNCTION_HPP__ */
