#include <iostream>
#include <iomanip>

#include "Block.hpp"

template<typename T>
class Matrix
{
 private:
    unsigned int mRowSize;
    unsigned int mColSize;
    T* mArray;

 public:
     Matrix(unsigned int rows, unsigned int cols) : mRowSize(cols), mColSize(rows), mArray(new T[mRowSize * mColSize])
     {
         std::cout << "Ctr default" << std::endl;
     }

     Matrix(unsigned int rows, unsigned int cols, std::initializer_list<T> ilist) : Matrix(rows, cols)
     {
         unsigned int i = 0;

         for (const T& n: ilist)
         {
             mArray[i++] = n;
         }
     }

     Matrix(const Matrix& other) : Matrix(other.mColSize, other.mRowSize)
     {
         std::cout << "Ctr copy" << std::endl;
         for (unsigned int i = 0; i < (mColSize * mRowSize); ++i)
         {
             mArray[i] = other.mArray[i];
         }
     }

     Matrix(Matrix&& other) : mRowSize(other.mRowSize), mColSize(other.mColSize), mArray(other.mArray)
     {
         std::cout << "Ctr move" << std::endl;
         other.mArray = nullptr;
     }

     Matrix& operator= (Matrix lhs)
     {
         std::swap(mColSize, lhs.mColSize);
         std::swap(mRowSize, lhs.mRowSize);
         std::swap(mArray, lhs.mArray);

         return *this;
     }

     Matrix& operator= (std::initializer_list<T> ilist)
     {
         unsigned int i = 0;

         for (const T& n: ilist)
         {
             mArray[i++] = n;
         }

         return *this;
     }

     unsigned int getRowSize() const
     {
         return mRowSize;
     }

     unsigned int getColSize() const
     {
         return mColSize;
     }

     const Block<T> row (unsigned int i) const
     {
         return {mArray + mRowSize * i, mRowSize, 1};
     }

     Block<T> row (unsigned int i)
     {
         return {mArray + mRowSize * i, mRowSize, 1};
     }

     const Block<T> col (unsigned int i) const
     {
         return {mArray + i, mColSize, mRowSize};
     }

     Block<T> col (unsigned int i)
     {
         return {mArray + i, mColSize, mRowSize};
     }

     T& operator() (unsigned int i, unsigned int j)
     {
         return mArray[i * mRowSize + j];
     }

     T operator() (unsigned int i, unsigned int j) const
     {
         return mArray[i * mRowSize + j];
     }

     ~Matrix()
     {
         std::cout << "Dtr" << std::endl;
         delete [] mArray;
     }
};

template <typename T>
std::ostream& operator<< (std::ostream &os, const Matrix<T>& m)
{
    for (unsigned int i = 0; i < m.getColSize(); ++i)
    {
        os << '[';

        bool first = true;

        for (unsigned int j = 0; j < m.getRowSize(); ++j)
        {
            if (!first) os << ","; else first = false;
            os << std::setprecision(1) << std::setw(5) << m(i, j);
        }

        os << ']' << std::endl;
    }

    return os;
}

template<typename T>
void eliminate (Matrix<T>& m)
{
    for (unsigned int i = 0; i < m.getColSize(); ++i)
    {
        m.row(i) /= m(i, i);

        for (unsigned int j = i + 1; j < m.getColSize(); ++j)
        {
            m.row(j) -= m.row(i) * m(j, i);
        }

        std::cout << m << std::endl;
    }
}
