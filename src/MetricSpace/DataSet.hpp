#ifndef __DATASET_HPP__
#define __DATASET_HPP__

template<typename PointType, typename PointTypeRef = PointType&>
class DataSet
{
 public:
    using Point = PointType;
    using PointRef = PointTypeRef;

    virtual Point operator[] (unsigned int i) const = 0;

    virtual unsigned int getPointNum () const = 0;
};

#endif /* end of include guard: __DATASET_HPP__ */
