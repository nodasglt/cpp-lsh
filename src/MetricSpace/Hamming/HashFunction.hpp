#ifndef __METRICSPACE_HAMMING_HASHFUNCTION_HPP__
#define __METRICSPACE_HAMMING_HASHFUNCTION_HPP__

#include "Containers/Array.hpp"
#include "Containers/Matrix.hpp"
#include "../Generic/HashFunction.hpp"
#include "Hamming.hpp"

namespace MetricSpace
{
    namespace Hamming
    {
        struct HashFunction : public Generic::HashFunction<DataPoint>
        {
            HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, unsigned int bitArrayLength);

            uint64_t getKeyAtIndex (ConstPointRef p, unsigned int i) const override;

         private:
            Matrix<std::size_t> returnBitIndex;
        };
    }
}

#endif /* end of include guard: __METRICSPACE_HAMMING_HASHFUNCTION_HPP__ */
