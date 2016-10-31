#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "MetricSpace/Euclidean/DataSet.hpp"
#include "MetricSpace/Hamming/DataSet.hpp"
#include "MetricSpace/DistMatrix/DataSet.hpp"

namespace Parser
{
    enum class Flags { euclidean, cosine, none, test };

    template<typename DataSetType>
    struct Result
    {
        DataSetType dataSet;
        Flags metric;
        double radius;
    };

    std::string getMetricSpace(const std::string& fileName);

    template<typename DataSetType>
    Result<DataSetType> parse(const std::string& filename);

    template<>
    Result<MetricSpace::Euclidean::DataSet> parse<MetricSpace::Euclidean::DataSet>(const std::string& fileName);

    template<>
    Result<MetricSpace::Hamming::DataSet> parse<MetricSpace::Hamming::DataSet>(const std::string& fileName);

    template<>
    Result<MetricSpace::DistMatrix::DataSet> parse<MetricSpace::DistMatrix::DataSet>(const std::string& fileName);
}

#endif /* end of include guard: __PARSER_HPP__ */
