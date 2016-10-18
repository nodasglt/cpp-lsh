#ifndef __GENERIC_DATASET_HPP__
#define __GENERIC_DATASET_HPP__

namespace MetricSpace {
namespace Generic
{
    template<typename PointType>
    class DataSet
    {
     public:
        using Point = typename PointType::Type;
        using ConstPointRef = typename PointType::ConstRefType;

        virtual ConstPointRef operator[] (unsigned int i) const = 0;

        virtual unsigned int getPointNum () const = 0;

        virtual ~DataSet() {}
    };
}}

#endif /* end of include guard: __GENERIC_DATASET_HPP__ */
