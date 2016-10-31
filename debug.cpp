#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include "MetricSpace/Euclidean/L2/Metric.hpp"
#include "MetricSpace/Euclidean/Cos/Metric.hpp"
#include "MetricSpace/Hamming/Metric.hpp"
#include "MetricSpace/DistMatrix/Metric.hpp"
#include "Clustering/LocSenHash.hpp"
#include "Clustering/BruteForceNN.hpp"
#include "Parser.hpp"

using namespace MetricSpace;

bool gPrintLSH = false;
bool gPrintBF = false;

double execTime (std::function<void (void)> func)
{
    std::clock_t begin = std::clock();

    func();

    std::clock_t end = std::clock();
    return double(end - begin) / CLOCKS_PER_SEC;
}

template<typename DataPointType>
void runQuery (Clustering::BruteForceNN<DataPointType>& bf,
               Clustering::LocSenHash<DataPointType>& lsh,
               Generic::DataSet<DataPointType>& testSet,
               double R)
{
//========= LSH ======================================================================================================================
    int ok = 0;
    unsigned int sum = 0;

    auto elapsedSecs = execTime([&]()
    {
        if (gPrintLSH)
            std::cout << "   NN         |   lsh   | true | status|cluster size" << std::endl;

        for (unsigned int t = 0; t < testSet.getPointNum(); ++t)
        {
            auto result = lsh(testSet[t]);

            if (gPrintLSH)
            {
                auto actual = bf(testSet[t]);

                if (result.found)
                {
                    if (result.dist == actual.dist) ++ok;
                    std::cout << std::setw(3) << t << " -> " << std::setw(3) << result.index
                    << "\t" << std::setw(6) << std::setprecision(2) << result.dist << std::setw(6) << std::setprecision(2) << actual.dist
                    << "\t[" << ((result.dist == actual.dist) ? "OK]\t" : "_]\t") << result.sum << std::endl;
                }
                else
                {
                    std::cout << std::setw(3) << t << " -> \t\t\t\t[FAIL]\t" << result.sum << std::endl;
                }

                sum += result.sum;
            }
        }
    });

    std::cout << "time: " << elapsedSecs << "\tqps: " << 1 / (elapsedSecs / testSet.getPointNum()) << std::endl;

    if (gPrintLSH)
        std::cout << "stats: " << (100 * (testSet.getPointNum() - (double)ok))/testSet.getPointNum() << "% fail rate\tcluster size avg: " << (double)sum / testSet.getPointNum() << std::endl;

//========= Brute Force ==============================================================================================================

    auto elapsedSecsBF = execTime([&]()
    {
        for (unsigned int t = 0; t < testSet.getPointNum(); ++t)
        {
            auto actual = bf(testSet[t]);

            if (gPrintBF)
                std::cout << "NN : " << t << " -> " << actual.index << "\tdist: " << actual.dist << std::endl;
        }
    });

    std::cout << "time: " << elapsedSecsBF << "\tqps: " << 1 / (elapsedSecsBF / testSet.getPointNum()) << std::endl;
}

template<typename DataPointType, typename DataSetType>
void inputLoop (const DataSetType& dataSet,
                const Generic::HashFunction<DataPointType>& hashFunc,
                const Generic::DistanceFunction<DataPointType>& distFunc,
                std::string queryFileName)
{
    Clustering::LocSenHash<DataPointType> lsh(hashFunc, distFunc, 125, dataSet);
    Clustering::BruteForceNN<DataPointType> bf(distFunc, dataSet);

    while (true)
    {
        try
        {
            auto result = Parser::parse<DataSetType>(queryFileName);

            if (result.metric == Parser::Flags::test)
            {
                runQuery(bf, lsh, result.dataSet, result.radius);
            }
            else
            {
                std::cout << "Expecting query file. Data file was given." << std::endl;
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cout << e.what() << std::endl;
        }

        std::cout << "Repeat with different query? [y/N]: ";
        std::string responce;
        std::cin >> responce;
        if (responce == "y" || responce == "Y")
        {
            std::cout << "Query file path: ";
            std::cin >> queryFileName;
        }
        else break;
    }
}

void parseArgs (int argc, char const* argv[],
                std::string& dataFileName,
                std::string& queryFileName,
                int& hashTableNum,
                int& functionsPerHashTable);

int main(int argc, char const* argv[])
{
    std::srand(std::time(nullptr));

    std::string dataFileName = "";
    std::string queryFileName = "";

    int hashTableNum = 6, functionsPerHashTable = 4;

    parseArgs(argc, argv, dataFileName, queryFileName,
              hashTableNum, functionsPerHashTable);

    try {

    std::string metricSpace = Parser::getMetricSpace(dataFileName);

    if (metricSpace == "euclidean")
    {
        using namespace Euclidean;

        auto result = Parser::parse<DataSet>(dataFileName);

        if (result.metric == Parser::Flags::euclidean)
        {
            inputLoop( result.dataSet,
                       L2::HashFunction( hashTableNum,
                                         functionsPerHashTable,
                                         result.dataSet.getVectorDim(),
                                         8.0f ),
                      L2::DistanceFunction(),
                      queryFileName );
        }
        else if (result.metric == Parser::Flags::cosine)
        {
            inputLoop( result.dataSet,
                       Cos::HashFunction( hashTableNum,
                                          functionsPerHashTable,
                                          result.dataSet.getVectorDim() ),
                       Cos::DistanceFunction(),
                       queryFileName );
        }
        else
        {
            std::cout << "Metric not supported." << std::endl;
            return 3;
        }
    }
    else if (metricSpace == "hamming")
    {
        using namespace Hamming;

        auto result = Parser::parse<DataSet>(dataFileName);

        if (result.metric == Parser::Flags::none)
        {
            inputLoop( result.dataSet,
                       HashFunction( hashTableNum,
                                     functionsPerHashTable,
                                     result.dataSet.getVectorDim() ),
                       DistanceFunction(),
                       queryFileName );
        }
        else
        {
            std::cout << "Metric not supported." << std::endl;
            return 3;
        }
    }
    else if (metricSpace == "matrix")
    {
        using namespace DistMatrix;

        auto result = Parser::parse<DataSet>(dataFileName);

        if (result.metric == Parser::Flags::none)
        {
            DistanceFunction distFunc;

            inputLoop( result.dataSet,
                       HashFunction( hashTableNum,
                                     functionsPerHashTable,
                                     distFunc,
                                     result.dataSet ),
                       distFunc,
                       queryFileName );
        }
        else
        {
            std::cout << "Metric not supported." << std::endl;
            return 3;
        }
    }
    else
    {
        std::cout << "Incompatible data file. Unknown metric space: " + metricSpace + ")" << std::endl;
        return 4;
    }

    } catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return 9;
    }

    return 0;
}

void parseArgs (int argc, char const* argv[],
                std::string& dataFileName,
                std::string& queryFileName,
                int& hashTableNum,
                int& functionsPerHashTable)
{
    for (; argc > 1; argc -= 2)
    {
        std::string flag(argv[argc - 2]);
        std::string value(argv[argc - 1]);

        if (flag == "-d")
        {
            dataFileName = value;
        }
        else if (flag == "-q")
        {
            queryFileName = value;
        }
        else if (flag == "-L")
        {
            hashTableNum = std::stoi(value);
        }
        else if (flag == "-k")
        {
            functionsPerHashTable = std::stoi(value);
        }
        else if (value == "-preal")
        {
            gPrintBF = true;
            argc++;
        }
        else if (value == "-plsh")
        {
            gPrintLSH = true;
            argc++;
        }
        else
        {
            std::cout << "Invalid flag: " << flag << std::endl;
            assert(0);
        }
    }

    if (dataFileName == "")
    {
        std::cout << "Data file path: ";
        std::cin >> dataFileName;
    }

    if (queryFileName == "")
    {
        std::cout << "Query file path: ";
        std::cin >> queryFileName;
    }
}
