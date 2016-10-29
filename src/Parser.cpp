#include "Parser.hpp"

namespace Parser
{
    unsigned getVectorNum(const std::string& fileName)
    {
       std::ifstream file(fileName);

       file.unsetf(std::ios_base::skipws);

       return std::count(std::istream_iterator<char>(file), std::istream_iterator<char>(), '\n');
    }

    unsigned getVectorDim(const std::string& fileName, int skipNum)
    {
       std::ifstream file(fileName);

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
    Result<MetricSpace::Euclidean::DataSet> parse<MetricSpace::Euclidean::DataSet>(const std::string& fileName)
    {
       std::ifstream file(fileName);

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
           if (metricSpace != "euclidean") throw;

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
           else throw;

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
    Result<MetricSpace::Hamming::DataSet> parse<MetricSpace::Hamming::DataSet>(const std::string& fileName)
    {
        std::ifstream file(fileName);

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
            if (metricSpace != "hamming") throw;

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
    Result<MetricSpace::DistMatrix::DistanceFunction> parse<MetricSpace::DistMatrix::DistanceFunction>(const std::string& fileName)
    {
        std::ifstream file(fileName);

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

            return {std::move(vecs), returnFlags, R};
        }
        else
        {
            file >> metricSpace;
            if (metricSpace != "matrix") throw;

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

            return {std::move(vecs), returnFlags, R};
        }
    }
}
