#ifndef __METRICSPACE_DISTMATRIX_DATASET_HPP__
#define __METRICSPACE_DISTMATRIX_DATASET_HPP__

#include "Containers/Matrix.hpp"
#include "../Generic/DataSet.hpp"
#include "DistMatrix.hpp"

namespace MetricSpace {
namespace DistMatrix
{
    class DataSet : public Generic::DataSet<DataPoint>
    {
     private:
        DataPoint::Flag mFlag;
        Matrix<double> mDistances;

     public:
         DataSet(DataPoint::Flag flag, Matrix<double>&& m);

         DataSet(DataSet&& other);
         DataSet(const DataSet& other);

         DataSet& operator= (DataSet other);

         ConstPointRef operator[] (unsigned int i) const override;

         unsigned int getPointNum () const override;
    };
}}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_DATASET_HPP__ */
