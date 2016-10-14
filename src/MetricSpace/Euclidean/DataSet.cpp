#include "DataSet.hpp"

using namespace MetricSpace::Euclidean;

DataSet::DataSet(Matrix<double>&& m) : mVectors(std::move(m)) {}

DataSet::DataSet(DataSet&& other) : mVectors(std::move(other.mVectors)) {}

DataSet::DataSet(const DataSet& other) : mVectors(other.mVectors) {}

DataSet& DataSet::operator= (DataSet other)
{
    std::swap(mVectors, other.mVectors);

    return *this;
}

Block<double*, double> DataSet::operator[] (unsigned int i) const
{
    return mVectors.row(i);
}

unsigned int DataSet::getPointNum() const
{
    return mVectors.getColSize();
}

unsigned int DataSet::getVectorDim() const
{
    return mVectors.getRowSize();
}


DataSetParser::DataSetParser () : mMetric(Metric::undefined) {}

DataSet DataSetParser::parse (const std::string& fileName)
{
   std::ifstream file(fileName);

   /* check metric space type */
   std::string metricSpace;
   file >> metricSpace >> metricSpace;
   if (metricSpace != "euclidean") throw;

   /* store requested metric */
   std::string metric;
   file >> metric >> metric;
   if (metric == "euclidean")
   {
       mMetric = Metric::euclidean;
   }
   else throw;

   Matrix<double> vecs(getVectorNum(fileName), getVectorDim(fileName));

   for (unsigned int i = 0; i < vecs.getColSize(); ++i)
   {
       std::string id;
       file >> id;
       //std::cout << id << std::endl;
       std::istream_iterator<double> curValue(file);
       for (unsigned int j = 0; ; ++j, ++curValue)
       {
           vecs(i, j) = *curValue;
           if (j == vecs.getRowSize() - 1) break;
       }
   }

   return {std::move(vecs)};
}

Metric DataSetParser::getMetric () const
{
   return mMetric;
}


unsigned int DataSetParser::getVectorNum(const std::string& fileName) const
{
   std::ifstream file(fileName);

   file.unsetf(std::ios_base::skipws);

   return std::count(std::istream_iterator<char>(file), std::istream_iterator<char>(), '\n') - 2;
}

unsigned int DataSetParser::getVectorDim(const std::string& fileName) const
{
   std::ifstream file(fileName);

   std::string skip;
   file >> skip >> skip >> skip >> skip;

   file.unsetf(std::ios_base::skipws);

   unsigned int dim = 0;

   std::istream_iterator<char> to;
   std::istream_iterator<char> from(file);

   ++from;

   for( ; from != to && *from != '\n'; ++from)
   {
       if (*from == '\t')
       {
           ++dim;
       }
   }

   return dim;
}
