#ifndef LIB_MATRIX_H_INCLUDED
#define LIB_MATRIX_H_INCLUDED

#include <cstddef>
#include <iostream>
#include "../../1/lib/macro.h"

namespace cmath {

template <size_t N, typename ElementType>
class Vector {
public:
    Vector()
    {
        data_ = new ElementType[N];
        ASSERT(data_ != nullptr);
    }

    Vector(const Vector<N, ElementType> &vect)
    {
        data_ = new ElementType[N];
        ASSERT(data_ != nullptr);
        for (size_t i = 0; i < N; i++) {
            data_[i] = vect(i);
        }
    }

    ~Vector() {
        delete [] data_;
        data_ = nullptr;
    }

    const ElementType &operator()(size_t i) const
    {
        ASSERT(i < N);
        return data_[i];
    }

    ElementType &operator()(size_t i)
    {
        ASSERT(i < N);
        return data_[i];
    }

private:
    ElementType *data_{nullptr};
};

enum class MatrixType {
    COLUMN_BASED,
    ROW_BASED
};

template <size_t ROWS_N, size_t COLUMNS_N, MatrixType type, typename ElementType>
class Matrix;

template <size_t ROWS_N, size_t COLUMNS_N, typename ElementType>
class Matrix<ROWS_N, COLUMNS_N, MatrixType::ROW_BASED, ElementType> {
public:
    Matrix()
    {
        data_ = new ElementType *[ROWS_N];
        ASSERT(data_ != nullptr);
        for (size_t i = 0; i < ROWS_N; i++) {
            data_[i] = new ElementType[COLUMNS_N];
            ASSERT(data_[i] != nullptr);
        }
    }
    Matrix(const Matrix<ROWS_N, COLUMNS_N, MatrixType::ROW_BASED, ElementType> &matrix)
    {
        data_ = new ElementType *[ROWS_N];
        ASSERT(data_ != nullptr);
        ASSERT(matrix.data_ != nullptr);
        for (size_t i = 0; i < ROWS_N; i++) {
            data_[i] = new ElementType[COLUMNS_N];
            ASSERT(data_[i] != nullptr);
            ASSERT(matrix.data_[i] != nullptr);
            for (size_t j = 0; j < COLUMNS_N; j++) {
                data_[i][j] = matrix(i, j);
            }
        }
    }
    ~Matrix()
    {
        ASSERT(data_ != nullptr);
        for (size_t i = 0; i < ROWS_N; i++) {
            delete [] data_[i];
        }
        delete [] data_;
        data_ = nullptr;
    }
    
    constexpr size_t GetRowsN() const {
        return ROWS_N;
    }
    constexpr size_t GetColumnsN() const {
        return COLUMNS_N;
    }

    const Vector<COLUMNS_N, ElementType> &SolveGauss(const Vector<ROWS_N, ElementType> &vect)
    {
        Gauss(*this, vect);
        return vect;
    }

    const ElementType &operator()(size_t i, size_t j) const
    {
        ASSERT(i < ROWS_N);
        ASSERT(j < COLUMNS_N);
        return data_[i][j];
    }
    
    ElementType &operator()(size_t i, size_t j)
    {
        ASSERT(i < ROWS_N);
        ASSERT(j < COLUMNS_N);
        return data_[i][j];
    }

private:
    class Gauss {
    public:
        Gauss(const Matrix<ROWS_N, COLUMNS_N, MatrixType::ROW_BASED, ElementType> &matrix,
              const Vector<ROWS_N, ElementType> &vect) : matrix_(matrix), vect_(vect) {
            ASSERT(matrix_.GetRowsN() == matrix_.GetColumnsN());
            std::cout << matrix_ << std::endl;
        }

    private:
        Matrix<ROWS_N, COLUMNS_N, MatrixType::ROW_BASED, ElementType> matrix_;
        Vector<ROWS_N, ElementType> vect_;
    };

private:
    ElementType **data_{nullptr};

    template <size_t ROWS, size_t COLUMNS, typename ElementT>
    friend std::ostream& operator<<(std::ostream &ou, Matrix<ROWS, COLUMNS, MatrixType::ROW_BASED, ElementT> &matrix);
};

template <size_t ROWS, size_t COLUMNS, typename ElementT>
std::ostream& operator<<(std::ostream &out, Matrix<ROWS, COLUMNS, MatrixType::ROW_BASED, ElementT> &matrix)
{
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMNS; j++) {
            std::cout << matrix(i, j) << " ";
        }
        std::cout << std::endl;
    }
    return out;
}

}  // cmath

#endif  // LIB_MATRIX_H_INCLUDED
