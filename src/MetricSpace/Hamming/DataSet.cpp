#include "DataSet.hpp"

using namespace MetricSpace::Hamming;

DataSet::DataSet(Array<BitArray<64>>&& m, unsigned dim) : mVectors(std::move(m)), mDim(dim) {}

DataSet::DataSet(DataSet&& other) : mVectors(std::move(other.mVectors)) {}

DataSet::DataSet(const DataSet& other) : mVectors(other.mVectors) {}

DataSet& DataSet::operator= (DataSet other)
{
    std::swap(mVectors, other.mVectors);

    return *this;
}

DataSet::ConstPointRef DataSet::operator[] (unsigned int i) const
{
    return mVectors[i];
}

unsigned int DataSet::getPointNum() const
{
    return mVectors.getLength();
}

unsigned int DataSet::getVectorDim() const
{
    return mDim;
}
