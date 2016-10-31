#include "Parser.hpp"

namespace Parser
{
    using namespace MetricSpace;

    std::string getMetricSpace(const std::string& fileName)
    {
       std::ifstream file(fileName);

       if (!file.is_open()) throw std::runtime_error("File " + fileName + " cannot be oppened.");

       std::string metricSpace;

       file >> metricSpace >> metricSpace;

       return metricSpace;
    }

    unsigned getVectorNum(const std::string& fileName)
    {
       std::ifstream file(fileName);

       if (!file.is_open()) throw std::runtime_error("File " + fileName + " cannot be oppened.");

       file.unsetf(std::ios_base::skipws);

       return std::count(std::istream_iterator<char>(file), std::istream_iterator<char>(), '\n');
    }

    unsigned getVectorDim(const std::string& fileName, int skipNum)
    {
       std::ifstream file(fileName);

       if (!file.is_open()) throw std::runtime_error("File " + fileName + " cannot be oppened.");

       for (int i = 0; i < skipNum; ++i)
       {
           std::string skip;
           file >> skip;
       }

       file.unsetf(std::ios_base::skipws);

       unsigned int dim = 0;

       std::istream_iterator<char> to;
       std::istream_iterator<char> from(file);

       ++from;

       for( ; from != to && *from != '\n'; ++from)
       {
           if (*from == '\t')
           {
               ++dim;
           }
       }

       return dim;
    }

    template<>
    Result<Euclidean::DataSet> parse<Euclidean::DataSet>(const std::string& fileName)
    {
       std::ifstream file(fileName);

       if (!file.is_open()) throw std::runtime_error("File " + fileName + " cannot be oppened.");

       /* check metric space type */
       std::string metricSpace;
       file >> metricSpace;

       int skip;

       Flags returnFlags;
       double R = 0.0f;

       if (metricSpace == "Radius:")
       {
           file >> R;

           returnFlags = Flags::test;

           skip = 1;
       }
       else
       {
           file >> metricSpace;
           if (metricSpace != "euclidean") throw std::runtime_error("Incompatible data file: " + fileName);

           /* store requested metric */
           std::string metric;
           file >> metric >> metric;
           if (metric == "euclidean")
           {
               returnFlags = Flags::euclidean;
           }
           else if (metric == "cosine")
           {
               returnFlags = Flags::cosine;
           }
           else throw std::runtime_error("Incompatible data file: " + fileName + " (Unknown metric: " + metric + ")");

           skip = 2;
       }

       Matrix<double> vecs(getVectorNum(fileName) - skip, getVectorDim(fileName, 2 * skip));

       for (unsigned i = 0; i < vecs.getColSize(); ++i)
       {
           std::string id;
           file >> id;

           for (unsigned j = 0; ; ++j)
           {
               file >> vecs(i, j);
               if (j == vecs.getRowSize() - 1) break;
           }
       }

       return {std::move(vecs), returnFlags, R};
    }

    template<>
    Result<Hamming::DataSet> parse<Hamming::DataSet>(const std::string& fileName)
    {
        std::ifstream file(fileName);

        if (!file.is_open()) throw std::runtime_error("File " + fileName + " cannot be oppened.");

        std::string metricSpace;
        file >> metricSpace;

        Flags returnFlags;
        double R = 0.0f;

        if (metricSpace == "Radius:")
        {
            file >> R;

            returnFlags = Flags::test;
        }
        else
        {
            file >> metricSpace;
            if (metricSpace != "hamming") throw std::runtime_error("Incompatible data file: " + fileName);

            returnFlags = Flags::none;
        }

        Array<BitArray<64>> vecs(getVectorNum(fileName) - 1);

        unsigned dim = 0;

        for (unsigned i = 0; i < vecs.getLength(); ++i)
        {
            std::string id;
            file >> id;

            std::string bits;
            file >> bits;
            dim = bits.length();
            for(unsigned j = 0; j < bits.length(); ++j)
            {
                switch (bits[j])
                {
                    case '0':
                        break;

                    case '1':
                        vecs[i].set(j);
                        break;

                    default:
                        assert(0);
                }
            }
        }

        return {{std::move(vecs), dim}, returnFlags, R};
    }

    template<>
    Result<DistMatrix::DataSet> parse<DistMatrix::DataSet>(const std::string& fileName)
    {
        std::ifstream file(fileName);

        if (!file.is_open()) throw std::runtime_error("File " + fileName + " cannot be oppened.");

        std::string metricSpace;
        file >> metricSpace;

        Flags returnFlags;
        double R = 0.0f;

        if (metricSpace == "Radius:")
        {
            file >> R;

            returnFlags = Flags::test;

            Matrix<double> vecs(getVectorNum(fileName) - 1, getVectorDim(fileName, 2));

            for (unsigned i = 0; i < vecs.getColSize(); ++i)
            {
                std::string id;
                file >> id;

                for (unsigned j = 0; ; ++j)
                {
                    file >> vecs(i, j);
                    if (j == vecs.getRowSize() - 1) break;
                }
            }

            return { { DistMatrix::DataPoint::Flag::test, std::move(vecs) }, returnFlags, R };
        }
        else
        {
            file >> metricSpace;
            if (metricSpace != "matrix") throw std::runtime_error("Incompatible data file: " + fileName);

            returnFlags = Flags::none;

            auto dim = getVectorNum(fileName) - 2;
            Matrix<double> vecs(dim, dim);

            std::string id;
            file >> id;
            std::getline(file, id);

            for (unsigned i = 0; i < vecs.getColSize(); ++i)
            {
                for (unsigned j = 0; ; ++j)
                {
                    file >> vecs(i, j);
                    if (j == vecs.getRowSize() - 1) break;
                }
            }

            return { { DistMatrix::DataPoint::Flag::data, std::move(vecs) }, returnFlags, R };
        }
    }
}
