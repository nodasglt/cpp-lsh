#ifndef __METRICSPACE_DISTMATRIX_HASHFUNCTION_HPP__
#define __METRICSPACE_DISTMATRIX_HASHFUNCTION_HPP__

#include "Containers/Matrix.hpp"
#include "DistMatrix.hpp"
#include "../Generic/HashFunction.hpp"
#include "../Generic/DataSet.hpp"
#include "../Generic/DistanceFunction.hpp"

namespace MetricSpace
{
    namespace DistMatrix
    {
        class HashFunction : public Generic::HashFunction<DataPoint>
        {
        public:
            using DataSet = Generic::DataSet<DataPoint>;
            using DistanceFunction = Generic::DistanceFunction<DataPoint>;
        public:
            HashFunction(unsigned int hashTablesNum, unsigned int functionsPerHashTable, const DistanceFunction& dist, const DataSet& data);

            uint64_t getKeyAtIndex (ConstPointRef p, unsigned int i) const override;

         private:
            struct line
            {
                unsigned x, y;
                double length;
                double midValue;
            };

            const DataSet& mDataSet;
            const DistanceFunction& mDistFunc;

            Matrix<line> mLines;

            double project (line l, ConstPointRef indexToProject) const;
        };
    }
}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_HASHFUNCTION_HPP__ */
