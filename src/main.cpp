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
void run (Clustering::BruteForceNN<DataPointType>& bf,
        Clustering::LocSenHash<DataPointType>& lsh,
        Generic::DataSet<DataPointType>& testSet, double R,
        std::ostream& outStream)
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

void parseArgs (int argc, char const* argv[], std::string& dataFile, std::string& queryFile, std::string& outputFile, int &hashTableNum, int& functionsPerHashTable);

int main(int argc, char const* argv[])
{
    std::srand(std::time(nullptr));

    std::string dataFile;
    std::string queryFile;
    std::string outputFile = "";
    int hashTableNum = 6, functionsPerHashTable = 4;

    parseArgs(argc, argv, dataFile, queryFile, outputFile, hashTableNum, functionsPerHashTable);

    std::ofstream outputFileStream(outputFile);

    //Output to file. If no file name is given, output to stdout.
    std::ostream* outStream = (outputFileStream.is_open()) ? &outputFileStream : &std::cout;

    std::ifstream file(dataFile);
    if (!file.is_open())
    {
        std::cout << "Data file cannot be oppened." << std::endl;
        return 2;
    }
    std::string metricSpace;
    file >> metricSpace >> metricSpace;
    file.close();

    if (metricSpace == "euclidean")
    {
        using namespace Euclidean;

        auto testSet = Parser::parse<DataSet>(queryFile);

        if (testSet.metric != Parser::Flags::test) assert(0);

        auto dataSet = Parser::parse<DataSet>(dataFile);

        if (dataSet.metric == Parser::Flags::euclidean)
        {
            L2::HashFunction hashFunc(hashTableNum,functionsPerHashTable, dataSet.data.getVectorDim(), 8.0f);
            L2::DistanceFunction distFunc;
            Clustering::LocSenHash<DataPoint> lsh(hashFunc, distFunc, 125, dataSet.data);
            Clustering::BruteForceNN<DataPoint> bf(distFunc, dataSet.data);
            run(bf, lsh, testSet.data, testSet.radius, *outStream);
        }
        else if (dataSet.metric == Parser::Flags::cosine)
        {
            Cos::HashFunction hashFunc(hashTableNum, functionsPerHashTable, dataSet.data.getVectorDim());
            Cos::DistanceFunction distFunc;
            Clustering::LocSenHash<DataPoint> lsh(hashFunc, distFunc, 125, dataSet.data);
            Clustering::BruteForceNN<DataPoint> bf(distFunc, dataSet.data);
            run(bf, lsh, testSet.data, testSet.radius, *outStream);
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

        auto testSet = Parser::parse<DataSet>(queryFile);

        if (testSet.metric != Parser::Flags::test) assert(0);

        auto dataSet = Parser::parse<DataSet>(dataFile);

        if (dataSet.metric == Parser::Flags::none)
        {
            HashFunction hashFunc(hashTableNum,functionsPerHashTable, dataSet.data.getVectorDim());
            DistanceFunction distFunc;
            Clustering::LocSenHash<DataPoint> lsh(hashFunc, distFunc, 125, dataSet.data);
            Clustering::BruteForceNN<DataPoint> bf(distFunc, dataSet.data);
            run(bf, lsh, testSet.data, testSet.radius, *outStream);
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

        auto testDistFunc = Parser::parse<DistanceFunction>(queryFile);

        if (testDistFunc.metric != Parser::Flags::test) assert(0);

        auto distFunc = Parser::parse<DistanceFunction>(dataFile);

        if (distFunc.metric == Parser::Flags::none)
        {
            HashFunction hashFunc(hashTableNum,functionsPerHashTable, &distFunc.data);
            Clustering::LocSenHash<DataPoint> lsh(hashFunc, testDistFunc.data, 125, distFunc.data);
            hashFunc.setDistFunction(&testDistFunc.data);
            Clustering::BruteForceNN<DataPoint> bf(testDistFunc.data, distFunc.data);
            run(bf, lsh, testDistFunc.data, testDistFunc.radius, *outStream);
        }
        else
        {
            std::cout << "Metric not supported." << std::endl;
            return 3;
        }
    }
    else
    {
        std::cout << "Incompatible data file." << std::endl;
        return 4;
    }

    return 0;
}

void parseArgs (int argc, char const* argv[], std::string& dataFile, std::string& queryFile, std::string& outputFile, int &hashTableNum, int& functionsPerHashTable)
{
    if (argc < 5)
    {
        std::cout << "Insufficient arguments" << std::endl;
        assert(0);
    }

    for (; argc > 1; argc -= 2)
    {
        std::string flag(argv[argc - 2]);
        std::string value(argv[argc - 1]);

        if (flag == "-d")
        {
            dataFile = value;
        }
        else if (flag == "-q")
        {
            queryFile = value;
        }
        else if (flag == "-o")
        {
            outputFile = value;
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
}
