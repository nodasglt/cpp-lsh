#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include "MetricSpace/Euclidean/L2/Metric.hpp"
#include "MetricSpace/Euclidean/Cos/Metric.hpp"
#include "MetricSpace/Hamming/Metric.hpp"
#include "MetricSpace/DistMatrix/Metric.hpp"
#include "LocalitySensitiveHashing/HashSet.hpp"
#include "Parser.hpp"

using namespace MetricSpace;

struct QueryResult
{
    unsigned index;
    double dist;
};

double execTime (std::function<void (void)> func)
{
    std::clock_t begin = std::clock();

    func();

    std::clock_t end = std::clock();
    return double(end - begin) / CLOCKS_PER_SEC;
}

template<typename DataPointType>
QueryResult bruteForce (typename DataPointType::Type p, Generic::DataSet<DataPointType>& dataSet, Generic::DistanceFunction<DataPointType>& distFunc)
{
    double dist = 0;
    unsigned index = 0;

    {
        bool found = false;

        for (unsigned i = 0; i < dataSet.getPointNum(); ++i)
        {
            double curDist = distFunc(p, dataSet[i]);
            if ((curDist < dist || !found) && curDist > 0.0f)
            {
                found = true;
                dist = curDist;
                index = i;
            }
        }
    }

    return { index, dist };
}

template<typename DataPointType>
void run (Generic::DataSet<DataPointType>& dataSet,
        lsh::HashSet<DataPointType>& hashSet,
        Generic::DistanceFunction<DataPointType>& distFunc,
        Generic::DataSet<DataPointType>& testSet,
        bool printLSH, bool printBF)
{
    int ok = 0;
    unsigned int sum = 0;

    auto elapsedSecs = execTime([&]()
    {
        for (unsigned int t = 0; t < testSet.getPointNum(); ++t)
        {
            auto result = hashSet[testSet[t]];

            if (printLSH)
            {
                QueryResult actual = bruteForce(testSet[t], dataSet, distFunc);

                if (result.found)
                {
                    //std::cout << "#2" << std::endl;
                    auto ddd = distFunc(testSet[t], dataSet[result.index]);
                    if (result.index == actual.index) ++ok;
                    std::cout << "NN : " << t << " -> " << result.index << " (true: " << actual.dist << ")" << "\tdist: " << ddd <<
                    "\t[" << ((result.index == actual.index) ? "OK]" : "_]") << "\tsum: " << result.sum << std::endl;
                }
                else
                {
                    std::cout << "NN : " << t << " -> \t\t\t[FAIL]" << "\tsum: " << result.sum << std::endl;
                }

                sum += result.sum;
            }
        }
    });

    std::cout << "time: " << elapsedSecs << "\tqps: " << 1 / (elapsedSecs / testSet.getPointNum()) << std::endl;

    if (printLSH)
        std::cout << "stats: " << (100 * (testSet.getPointNum() - (double)ok))/testSet.getPointNum() << "% fail rate\tsum avg: " << (double)sum / testSet.getPointNum() << std::endl;

//========= Brute Force ================================================================================================================

    auto elapsedSecsBF = execTime([&]()
    {
        for (unsigned int t = 0; t < testSet.getPointNum(); ++t)
        {
            auto actual = bruteForce(testSet[t], dataSet, distFunc);

            if (printBF)
                std::cout << "NN : " << t << " -> " << actual.index << "\tdist: " << distFunc(testSet[t], dataSet[actual.index]) << std::endl;
        }
    });

    std::cout << "time: " << elapsedSecsBF << "\tqps: " << 1 / (elapsedSecsBF / testSet.getPointNum()) << std::endl;
}

int main(int argc, char const* argv[])
{
    std::srand(std::time(nullptr));

    bool printLSH = (std::string(argv[3]) == "true");
    bool printBF = (std::string(argv[4]) == "true");

    std::ifstream file(argv[1]);
    std::string metricSpace;
    file >> metricSpace >> metricSpace;
    file.close();

    if (metricSpace == "euclidean")
    {
        using namespace Euclidean;

        auto testSet = Parser::parse<DataSet>(argv[2]);

        //if (metric != Parser::Flags::test) assert(0);

        auto dataSet = Parser::parse<DataSet>(argv[1]);

        if (dataSet.metric == Parser::Flags::euclidean)
        {
            L2::HashFunction hashFunc(6, 4, dataSet.data.getVectorDim(), 8.0f);
            L2::DistanceFunction distFunc;
            lsh::HashSet<DataPoint> hashSet(hashFunc, distFunc, 125, dataSet.data);
            run(dataSet.data, hashSet, distFunc, testSet.data, printLSH, printBF);
        }
        else if (dataSet.metric == Parser::Flags::cosine)
        {
            Cos::HashFunction hashFunc(2, 20, dataSet.data.getVectorDim());
            Cos::DistanceFunction distFunc;
            lsh::HashSet<DataPoint> hashSet(hashFunc, distFunc, 125, dataSet.data);
            run(dataSet.data, hashSet, distFunc, testSet.data, printLSH, printBF);
        }
        else
        {
            assert(0);
        }
    }
    else if (metricSpace == "hamming")
    {
        using namespace Hamming;

        auto testSet = Parser::parse<DataSet>(argv[2]);

        //if (metric != Parser::Flags::test) assert(0);

        auto dataSet = Parser::parse<DataSet>(argv[1]);

        if (dataSet.metric == Parser::Flags::none)
        {
            HashFunction hashFunc(10, 5, dataSet.data.getVectorDim());
            DistanceFunction distFunc;
            lsh::HashSet<DataPoint> hashSet(hashFunc, distFunc, 125, dataSet.data);
            run(dataSet.data, hashSet, distFunc, testSet.data, printLSH, printBF);
        }
        else
        {
            assert(0);
        }
    }
    else if (metricSpace == "matrix")
    {
        using namespace DistMatrix;

        auto testDistFunc = Parser::parse<DistanceFunction>(argv[2]);

        //if (metric != Parser::Flags::test) assert(0);

        auto distFunc = Parser::parse<DistanceFunction>(argv[1]);

        if (distFunc.metric == Parser::Flags::none)
        {
            HashFunction hashFunc(6, 4, &distFunc.data);
            lsh::HashSet<DataPoint> hashSet(hashFunc, testDistFunc.data, 125, distFunc.data);
            hashFunc.setDistFunction(&testDistFunc.data);
            run(distFunc.data, hashSet, testDistFunc.data, testDistFunc.data, printLSH, printBF);
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
