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
               double R,
               std::ofstream& outStream)
{
    for (unsigned i = 0; i < testSet.getPointNum(); ++i)
    {
        outStream << "Query: item_idS" << (i + 1) << std::endl;

        outStream << "R-near neighbors:" << std::endl;
        lsh.forEachPointInRange(R + 0.5f, testSet[i], [&](auto index, auto)
        {
            outStream << "item" << (index + 1) << std::endl;
        });

        typename Clustering::LocSenHash<DataPointType>::QueryResult lshResult;
        auto timeLSH = execTime([&]()
        {
            lshResult = lsh(testSet[i]);
        });

        typename Clustering::BruteForceNN<DataPointType>::QueryResult trueResult;
        auto timeBF = execTime([&]()
        {
            trueResult = bf(testSet[i]);
        });

        if (lshResult.found)
        {
            outStream << "Nearest neighbor: item" << (lshResult.index + 1) << std::endl;
            outStream << "distanceLSH:\t" << lshResult.dist << std::endl;
        }
        else
        {
            outStream << "Nearest neighbor: n/a" << std::endl;
            outStream << "distanceLSH:\tn/a" << std::endl;
        }

        outStream << "distanceTrue:\t" << trueResult.dist << std::endl;
        outStream << "tLSH:\t" << std::fixed << (timeLSH * 1000.0f) << " ms" << std::endl;
        outStream << "tTrue:\t" << std::fixed << (timeBF * 1000.0f) << " ms" << std::endl << std::endl;
    }
}

template<typename DataPointType, typename DataSetType>
void inputLoop (const DataSetType& dataSet,
                const Generic::HashFunction<DataPointType>& hashFunc,
                const Generic::DistanceFunction<DataPointType>& distFunc,
                std::string queryFileName, std::ofstream outStream)
{
    if (!outStream.is_open())
    {
        std::cout << "Cannot create/open output file." << std::endl;
        return;
    }

    Clustering::LocSenHash<DataPointType> lsh(hashFunc, distFunc, 125, dataSet);
    Clustering::BruteForceNN<DataPointType> bf(distFunc, dataSet);

    while (true)
    {
        try
        {
            auto result = Parser::parse<DataSetType>(queryFileName);

            if (result.metric == Parser::Flags::test)
            {
                runQuery(bf, lsh, result.dataSet, result.radius, outStream);
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
                std::string& outputFileName,
                int& hashTableNum,
                int& functionsPerHashTable);

int main(int argc, char const* argv[])
{
    std::srand(std::time(nullptr));

    std::string dataFileName = "";
    std::string queryFileName = "";
    std::string outputFileName = "";

    int hashTableNum = 6, functionsPerHashTable = 4;

    parseArgs(argc, argv, dataFileName, queryFileName,
              outputFileName, hashTableNum, functionsPerHashTable);

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
                      queryFileName,
                      std::ofstream(outputFileName) );
        }
        else if (result.metric == Parser::Flags::cosine)
        {
            inputLoop( result.dataSet,
                       Cos::HashFunction( hashTableNum,
                                          functionsPerHashTable,
                                          result.dataSet.getVectorDim() ),
                       Cos::DistanceFunction(),
                       queryFileName,
                       std::ofstream(outputFileName) );
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
                       queryFileName,
                       std::ofstream(outputFileName) );
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
                       queryFileName,
                       std::ofstream(outputFileName) );
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
                std::string& outputFileName,
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
        else if (flag == "-o")
        {
            outputFileName = value;
        }
        else if (flag == "-L")
        {
            hashTableNum = std::stoi(value);
        }
        else if (flag == "-k")
        {
            functionsPerHashTable = std::stoi(value);
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

    if (outputFileName == "")
    {
        std::cout << "Output file path: ";
        std::cin >> outputFileName;
    }
}
