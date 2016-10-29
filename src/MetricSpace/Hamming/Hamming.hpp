#ifndef __METRICSPACE_HAMMING_HPP__
#define __METRICSPACE_HAMMING_HPP__

#include "Containers/BitArray.hpp"

namespace MetricSpace {
namespace Hamming
{
    struct DataPoint
    {
        //NOTE: Switch to Containers/DynamicBitArray if input length > 64 bits.
        using Type = BitArray<64>;
        using ConstRefType = const BitArray<64>&;
    };
}}

#endif /* end of include guard: __METRICSPACE_HAMMING_HPP__ */
