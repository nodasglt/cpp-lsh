#ifndef __VECTORSET_HPP__
#define __VECTORSET_HPP__

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <string>

#include "Containers/Matrix.hpp"
#include "MetricSpace/DataSet.hpp"

enum class Metric { undefined, euclidean, manhattan, cosine };

class VectorSetParser;

class VectorSet : public DataSet<Block<double*, double>, Block<double*, double>>
{
 private:
    Matrix<double> mVectors;

    VectorSet(Matrix<double>&& m);

 public:
     VectorSet(VectorSet&& other);
     VectorSet(const VectorSet& other);

     VectorSet& operator= (VectorSet other);

     Block<double*, double> operator[] (unsigned int i) const;

     unsigned int getPointNum() const;
     unsigned int getVectorDim() const;

     friend class VectorSetParser;
};

class VectorSetParser
{
 public:
    VectorSetParser ();

    VectorSet parse (const std::string& fileName);

    Metric getMetric () const;

 private:

    Metric mMetric;

    unsigned int getVectorNum(const std::string& fileName) const;
    unsigned int getVectorDim(const std::string& fileName) const;
};

#endif /* end of include guard: __VECTORSET_HPP__ */
