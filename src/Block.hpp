template<typename T>
class Block
{
 private:
    const unsigned int mOffset;
    const unsigned int mSize;
    T* const mArray;

 public:
     Block (T* const buffer, unsigned int size, unsigned int offset) : mOffset(offset), mSize(size), mArray(buffer) {}

     Block (const Block&) = default;

     unsigned int getSize() const
     {
         return mSize;
     }

     T begin() const
     {
         return mArray;
     }

     T* const begin()
     {
         return mArray;
     }

     T end() const
     {
         return mArray + mOffset * mSize;
     }

     T* const end()
     {
         return mArray + mOffset * mSize;
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
         if (mSize == vec.getSize())
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
