#ifndef __METRICSPACE_EUCLIDEAN_DATASET_HPP__
#define __METRICSPACE_EUCLIDEAN_DATASET_HPP__

#include <iostream>
#include <cstdlib>
#include <string>

#include "Containers/Matrix.hpp"
#include "../Generic/DataSet.hpp"
#include "DataPoint.hpp"

namespace MetricSpace {
namespace Euclidean
{
    class DataSet : public Generic::DataSet<DataPoint>
    {
     private:
        Matrix<double> mVectors;

     public:
         DataSet(Matrix<double>&& m);

         DataSet(DataSet&& other);
         DataSet(const DataSet& other);

         DataSet& operator= (DataSet other);

         ConstPointRef operator[] (unsigned int i) const override;

         unsigned int getPointNum () const override;
         unsigned int getVectorDim () const;
    };
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_DATASET_HPP__ */
