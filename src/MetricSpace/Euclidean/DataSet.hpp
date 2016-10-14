#ifndef __METRICSPACE_EUCLIDEAN_DATASET_HPP__
#define __METRICSPACE_EUCLIDEAN_DATASET_HPP__

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <string>

#include "../../Containers/Matrix.hpp"
#include "../Generic/DataSet.hpp"
#include "Euclidean.hpp"

namespace MetricSpace {
namespace Euclidean
{
    enum class Metric { undefined, euclidean, manhattan, cosine };

    class DataSetParser;

    class DataSet : public Generic::DataSet<DataPoint>
    {
     private:
        Matrix<double> mVectors;

        DataSet(Matrix<double>&& m);

     public:
         DataSet(DataSet&& other);
         DataSet(const DataSet& other);

         DataSet& operator= (DataSet other);

         Block<double*, double> operator[] (unsigned int i) const;

         unsigned int getPointNum() const;
         unsigned int getVectorDim() const;

         friend class DataSetParser;
    };

    class DataSetParser
    {
     public:
        DataSetParser ();

        DataSet parse (const std::string& fileName);

        Metric getMetric () const;

     private:

        Metric mMetric;

        unsigned int getVectorNum(const std::string& fileName) const;
        unsigned int getVectorDim(const std::string& fileName) const;
    };
}}

#endif /* end of include guard: __METRICSPACE_EUCLIDEAN_DATASET_HPP__ */
