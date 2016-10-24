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
    MetricSpace::Euclidean::DataSet parse<MetricSpace::Euclidean::DataSet>(const std::string& fileName, Flags& returnFlags)
    {
       std::ifstream file(fileName);

       /* check metric space type */
       std::string metricSpace;
       file >> metricSpace >> metricSpace;
       if (metricSpace != "euclidean") throw;

       /* store requested metric */
       std::string metric;
       file >> metric >> metric;
       if (metric == "euclidean")
       {
           returnFlags = Flags::euclidean;
       }
       else throw;

       Matrix<double> vecs(getVectorNum(fileName) - 2, getVectorDim(fileName, 4));

       for (unsigned i = 0; i < vecs.getColSize(); ++i)
       {
           std::string id;
           file >> id;

           //std::cout << id << std::endl;
           for (unsigned j = 0; ; ++j)
           {
               file >> vecs(i, j);
               if (j == vecs.getRowSize() - 1) break;
           }
       }

       return {std::move(vecs)};
    }

    template<>
    MetricSpace::Hamming::DataSet parse<MetricSpace::Hamming::DataSet>(const std::string& fileName, Flags& returnFlags)
    {
        std::ifstream file(fileName);

        std::string metricSpace;
        file >> metricSpace >> metricSpace;
        if (metricSpace != "hamming") throw;

        returnFlags = Flags::none;

        Array<BitArray<64>> vecs(getVectorNum(fileName) - 1);

        std::cout << vecs.getLength() << std::endl;

        for (unsigned i = 0; i < vecs.getLength(); ++i)
        {
            std::string id;
            file >> id;

            std::string bits;
            file >> bits;
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

            //std::cout << id << '\t' << vecs[i] << std::endl;
        }

        return {std::move(vecs)};
    }
}
