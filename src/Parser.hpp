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
    template<typename DataSetType>
    DataSetType parse(const std::string& filename);

    template<>
    MetricSpace::Euclidean::DataSet parse<MetricSpace::Euclidean::DataSet>(const std::string& fileName);

    template<>
    MetricSpace::Hamming::DataSet parse<MetricSpace::Hamming::DataSet>(const std::string& fileName);

//    template<>
//    MetricSpace::DistMatrix::DistanceFunction parse<MetricSpace::DistMatrix::DistanceFunction>(const std::string& fileName);
}

#endif /* end of include guard: __PARSER_HPP__ */
