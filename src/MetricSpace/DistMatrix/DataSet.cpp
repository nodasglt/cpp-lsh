#include "DataSet.hpp"

using namespace MetricSpace::DistMatrix;

DataSet::DataSet(DataPoint::Flag flag, Matrix<double>&& m) : mFlag(flag), mDistances(std::move(m)) {}

DataSet::DataSet(DataSet&& other) : DataSet(other.mFlag, std::move(other.mDistances)) {}

DataSet::DataSet(const DataSet& other) : mFlag(other.mFlag), mDistances(other.mDistances) {}

DataSet& DataSet::operator= (DataSet other)
{
    std::swap(mDistances, other.mDistances);

    return *this;
}

DataSet::ConstPointRef DataSet::operator[] (unsigned int i) const
{
    return { mFlag, i, mDistances.row(i) };
}

unsigned int DataSet::getPointNum() const
{
    return mDistances.getColSize();
}
