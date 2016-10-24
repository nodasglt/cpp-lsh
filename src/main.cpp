#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include "MetricSpace/Euclidean/L2/Metric.hpp"
#include "MetricSpace/Hamming/Metric.hpp"
#include "LocalitySensitiveHashing/HashSet.hpp"
#include "Parser.hpp"

using namespace MetricSpace;

template<typename DataPointType>
void run (Generic::DataSet<DataPointType>& dataSet, Generic::HashFunction<DataPointType>& hashFunc, Generic::DistanceFunction<DataPointType>& distFunc)
{
    lsh::HashSet<DataPointType> hashSet(hashFunc, distFunc, 125, dataSet);

    int ok = 0;

    unsigned int sum = 0;

    std::clock_t begin = std::clock();

    for (unsigned int t = 0; t < 1000; ++t)
    {
        auto result = hashSet[dataSet[t]];

        if (result.found)
        {
            std::cout << "NN : " << t << " -> " << result.index << "\tdist: " << distFunc(dataSet[result.index], dataSet[t]) << "\tsum: " << result.sum << std::endl;
        }
        else
        {
            //std::cout << "NN : " << t << " -> [FAIL]" << " sum: " << result.sum << std::endl;
        }

        sum += result.sum;

        if (result.found) ++ok;
    }

    std::clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "time: " << elapsed_secs << "\tqps: " << 1 / (elapsed_secs / dataSet.getPointNum()) << std::endl;

    std::cout << "stats: " << (100 * (dataSet.getPointNum() - (double)ok))/dataSet.getPointNum() << "% fail rate\tsum avg: " << (double)sum / dataSet.getPointNum() << std::endl;

//========= Brute Force ================================================================================================================

    std::clock_t begin_bf = std::clock();

    for (unsigned int t = 0; t < 1000; ++t)
    {
        bool found = false;
        double curMinDist = 0;
        unsigned index = 0;

        for (unsigned i = 0; i < dataSet.getPointNum(); ++i)
        {
            double dist = distFunc(dataSet[i], dataSet[t]);
            if ((dist < curMinDist || !found) && dist > 0/* NOTE: ignore existing values for testing -- remove when testSet is available */)
            {
                found = true;
                curMinDist = dist;
                index = i;
            }
        }


        //std::cout << "NN : " << t << " -> " << index << "\tdist: " << distFunc(dataSet[index], dataSet[t]) << std::endl;
    }

    std::clock_t end_bf = std::clock();
    double elapsed_secs_bf = double(end_bf - begin_bf) / CLOCKS_PER_SEC;

    std::cout << "time: " << elapsed_secs_bf << "\tqps: " << 1 / (elapsed_secs_bf / dataSet.getPointNum()) << std::endl;
}

int main(int argc, char const* argv[])
{
    std::srand(std::time(nullptr));

    std::ifstream file(argv[1]);
    std::string metricSpace;
    file >> metricSpace >> metricSpace;
    file.close();

    Parser::Flags metric;
    if (metricSpace == "euclidean")
    {
        using namespace Euclidean;

        auto dataSet = Parser::parse<DataSet>(argv[1], metric);

        if (metric == Parser::Flags::euclidean)
        {
            L2::HashFunction hashFunc(4, 6, 100, 1.5f);
            L2::DistanceFunction distFunc;
            run(dataSet, hashFunc, distFunc);
        }
        else
        {
            assert(0);
        }
    }
    else if (metricSpace == "hamming")
    {
        using namespace Hamming;

        auto dataSet = Parser::parse<DataSet>(argv[1], metric);

        if (metric == Parser::Flags::none)
        {
            HashFunction hashFunc(4, 10, 64);
            DistanceFunction distFunc;
            run(dataSet, hashFunc, distFunc);
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        assert(0);
    }

    return 0;
}

//        std::clock_t begin2 = std::clock();

//        for (unsigned int i = 0; i < dataSet.getPointNum(); ++i)
//        {
//            auto x = distFunc(dataSet[i], dataSet[t]);
//            if (x < 1300.0f)
//            {
//                if (x) std::cout << t << "| " << i << " : " << x << "\t" << ((found.contains(i)) ? "[OK]" : "[FAIL]") << std::endl;
//            }
//        }
//
//        std::clock_t end2 = std::clock();
//        double elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;

//        std::cout << "time: " << elapsed_secs2 << std::endl;

/*
int main (int argc, char* argv[])
{
    L2::HashFunction func(2, 4, 5, 2.0f);

    L2::Point p;

    p.reserve(5);

    for (unsigned int i = 0; i < 5; ++i)
    {
        p.pushBack((double)std::rand()/RAND_MAX + 5.0f);
    }

    std::cout << p << std::endl;

    for (auto x : func(p))
    {
        std::cout << x << std::endl;
    }

	return 0;
}
*/
