#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include <iostream>
#include <iomanip>
#include "Array.hpp"

template<typename BufferType, typename T>
class Block
{
 private:
    const unsigned int mOffset;
    const unsigned int mSize;
    BufferType mArray;

 public:
     Block (BufferType buffer, unsigned int size, unsigned int offset) : mOffset(offset), mSize(size), mArray(buffer) {}

     Block (Array<T>& array) : mOffset(1), mSize(array.getLength()), mArray((array.getLength()) ? &array[0] : nullptr) {}

     Block (const Block&) = default;

     unsigned int getLength() const
     {
         return mSize;
     }

     class iterator
     {
      private:
         BufferType mData;
         unsigned int mCurrentIndex;
         const unsigned int mOffset;

      public:

         iterator(BufferType data, unsigned int initIndex, unsigned int offset) : mData(data), mCurrentIndex(initIndex), mOffset(offset) {}

         bool operator!= (iterator other) const
         {
             return (mCurrentIndex != other.mCurrentIndex);
         }

         void operator++ ()
         {
             mCurrentIndex += mOffset;
         }

         T operator* () const
         {
             return mData[mCurrentIndex];
         }

         T& operator* ()
         {
             return mData[mCurrentIndex];
         }
     };

     const iterator begin() const
     {
         return {mArray, 0, mOffset};
     }

     iterator begin()
     {
         return {mArray, 0, mOffset};
     }

     const iterator end() const
     {
         return {mArray, mOffset * mSize, mOffset};
     }

     iterator end()
     {
         return {mArray, mOffset * mSize, mOffset};
     }

     T operator[] (unsigned int i) const
     {
         return mArray[i * mOffset];
     }

     T& operator[] (unsigned int i)
     {
         return mArray[i * mOffset];
     }

     template<typename VectorType>
     Block& operator= (const VectorType& vec)
     {
         if (mSize == vec.getLength())
         {
             for (int i = 0; i < mSize; ++i)
             {
                 (*this)[i] = vec[i];
             }
         }
     }

     Block& operator*= (T x)
     {
         for (T& n : *this)
         {
             n *= x;
         }
         return *this;
     }

     Block& operator/= (T x)
     {
         for (T& n : *this)
         {
             n /= x;
         }
         return *this;
     }

     Block& operator+= (T x)
     {
         for (T& n : *this)
         {
             n += x;
         }
         return *this;
     }

     Block& operator-= (T x)
     {
         for (T& n : *this)
         {
             n -= x;
         }
         return *this;
     }

     //================= Block -> Block =======================

     template<typename VectorType>
     Block& operator*= (const VectorType& other)
     {
         /* throw exception for dif sizes */
         for (unsigned int i = 0; i < mSize; ++i)
         {
             (*this)[i] *= other[i];
         }
         return *this;
     }

     template<typename VectorType>
     Block& operator/= (const VectorType& other)
     {
         /* throw exception for dif sizes */
         for (unsigned int i = 0; i < mSize; ++i)
         {
             (*this)[i] /= other[i];
         }
         return *this;
     }

     template<typename VectorType>
     Block& operator+= (const VectorType& other)
     {
         /* throw exception for dif sizes */
         for (unsigned int i = 0; i < mSize; ++i)
         {
             (*this)[i] += other[i];
         }
         return *this;
     }

     template<typename VectorType>
     Block& operator-= (const VectorType& other)
     {
         /* throw exception for dif sizes */
         for (unsigned int i = 0; i < mSize; ++i)
         {
             (*this)[i] -= other[i];
         }
         return *this;
     }
};

template<typename BufferType, typename T>
std::ostream& operator<< (std::ostream &os, const Block<BufferType, T>& m)
{
    os << '[';

    bool first = true;

    for (auto x : m)
    {
        if (!first) os << ","; else first = false;
        os << std::setw(5) << x;
    }

    os << ']' << std::endl;

    return os;
}

#endif /* end of include guard: __BLOCK_HPP__ */
