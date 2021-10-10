#ifndef LIB_MATRIX_H_INCLUDED
#define LIB_MATRIX_H_INCLUDED

#include <cstddef>
#include "../../1/lib/macro.h"

namespace cmath {

enum class MatrixType {
    COLUMN_BASED,
    ROW_BASED
};

template <size_t ROWS_N, size_t COLUMNS_N, MatrixType type, typename ElementType>
class Matrix;

template <size_t ROWS_N, size_t COLUMNS_N, typename ElementType>
class Matrix<ROWS_N, COLUMNS_N, MatrixType::COLUMN_BASED, ElementType>
{
public:
    Matrix()
    {
        data_ = new ElementType *[COLUMNS_N];
        ASSERT(data_ != nullptr);
        for (size_t i = 0; i < COLUMNS_N; i++) {
            data_[i] = new ElementType[ROWS_N];
            ASSERT(data_[i] != nullptr);
        }
    }

    const ElementType &operator()(size_t i, size_t j) const
    {
        ASSERT(i < ROWS_N);
        ASSERT(j < COLUMNS_N);
        return data_[j][i];
    }
    
    ElementType &operator()(size_t i, size_t j)
    {
        ASSERT(i < ROWS_N);
        ASSERT(j < COLUMNS_N);
        return data_[j][i];
    }

private:
    ElementType **data_{nullptr};
};

}  // cmath

#endif  // LIB_MATRIX_H_INCLUDED
