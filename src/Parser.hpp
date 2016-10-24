#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "MetricSpace/Euclidean/DataSet.hpp"
#include "MetricSpace/Hamming/DataSet.hpp"

namespace Parser
{
    enum class Flags { euclidean, cosine, none };

    template<typename DataSetType>
    DataSetType parse(const std::string& filename, Flags& returnFlags);

    template<>
    MetricSpace::Euclidean::DataSet parse<MetricSpace::Euclidean::DataSet>(const std::string& fileName, Flags& returnFlags);

    template<>
    MetricSpace::Hamming::DataSet parse<MetricSpace::Hamming::DataSet>(const std::string& fileName, Flags& returnFlags);

//    template<>
//    MetricSpace::DistMatrix::DistanceFunction parse<MetricSpace::DistMatrix::DistanceFunction>(const std::string& fileName, Flags& returnFlags);
}

#endif /* end of include guard: __PARSER_HPP__ */
