#ifndef LIB_MATRIX_H_INCLUDED
#define LIB_MATRIX_H_INCLUDED

#include <cstddef>
#include <iostream>
#include <utility>
#include "../../1/lib/macro.h"
#include "../../1/lib/log.h"

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
    
    void SwapElemets(size_t i, size_t j)
    {
        ASSERT(data_ != nullptr);
        ASSERT(i < N);
        ASSERT(j < N);
        ASSERT(data_ != nullptr);

        std::swap(data_[i], data_[j]);
    }

private:
    ElementType *data_{nullptr};
    template <size_t N_ELEM, typename ElementT>
    friend std::ostream& operator<<(std::ostream &ou, const Vector<N_ELEM, ElementT> &v);
};

template <size_t N_ELEM, typename ElementT>
std::ostream& operator<<(std::ostream &out, const Vector<N_ELEM, ElementT> &v)
{
    for (size_t i = 0; i < N_ELEM; i++) {
        out << v.data_[i] << " ";
    }
    return out;
}

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
    void SwapRows(size_t i, size_t j)
    {
        std::swap(data_[i], data_[j]);
    }
    auto FindMaxAbsElement() {
        ElementType max_abs = 0;
        size_t max_i = 0;
        size_t max_j = 0;
        for (size_t i = 0; i < ROWS_N; i++) {
            for (size_t j = 0; j < COLUMNS_N; j++) {
                if (max_abs < std::abs(data_[i][j])) {
                    max_abs = std::abs(data_[i][j]);
                    max_i = i;
                    max_j = j;
                }
            }
        }
        return std::make_pair(max_i, max_j); 
    }

    Vector<COLUMNS_N, ElementType> SolveGauss(const Vector<ROWS_N, ElementType> &vect)
    {
        return Gauss(*this, vect).GetAnsVect();
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
            ForwardElimination();
            BackwardElimination();
        }

        void ForwardElimination() {
            auto [i, j] = matrix_.FindMaxAbsElement();
            LOG_MATRIX(DEBUG, "Start ForwardElimination():\nmatrix:\n" << matrix_ <<
                              "vector:\n" << vect_ << '\n');
            ForwardStepRec(0, i, j);
            LOG_MATRIX(DEBUG, "End ForwardElimination():\nmatrix:\n" << matrix_ <<
                              "vector:\n" << vect_ << '\n');                              
        }

        void ForwardStepRec(size_t start_row_idx, size_t max_abs_i, size_t max_abs_j)
        {
            // Move the "based" row above in the matrix:
            if (start_row_idx != max_abs_i) {
                SwapRows(start_row_idx, max_abs_i);
            }
            // Normalize the "based" row:
            auto [new_max_i, new_max_j] = SubBasedRow(start_row_idx, max_abs_j);
            if (start_row_idx != (ROWS_N - 2)) {
                ForwardStepRec(start_row_idx + 1, new_max_i, new_max_j);
            }
        }

        void BackwardElimination()
        {
            LOG_MATRIX(DEBUG, "Start BackwardElimination();")
            
            for (ssize_t i = ROWS_N - 2; i >= 0; i--) {
                ans_map_(i + 1) = FindNonZeroElement(static_cast<size_t>(i + 1));
                for (size_t j = i + 1; j < ROWS_N; j++) {
                    ASSERT(matrix_(j, ans_map_(j)) != 0);
                    ElementType alpha = matrix_(i, ans_map_(j)) / matrix_(j, ans_map_(j));
                    matrix_(i, ans_map_(j)) = 0;
                    vect_(i) = vect_(i) - alpha * vect_(j);
                }
            }
            ans_map_(0) = FindNonZeroElement(static_cast<size_t>(0));

            LOG_MATRIX(DEBUG, "End ForwardElimination():\nmatrix:\n" << matrix_ <<
                              "vector:\n" << vect_ << '\n');                       
        }

        size_t FindNonZeroElement(size_t row_idx)
        {
            for (size_t i = 0; i < COLUMNS_N; i++) {
                if (matrix_(row_idx, i) != 0) {
                    return i;
                }
            }
            UNREACHABLE();
        }

        auto SubBasedRow(size_t start_row, size_t max_abs_j) {
            ASSERT(start_row < ROWS_N - 1);
            ASSERT(ROWS_N == COLUMNS_N);
            
            ElementType new_max_abs = 0;
            size_t new_max_i = 0;
            size_t new_max_j = 0;
            LOG_MATRIX(DEBUG, "Matrix before sub:\n" << matrix_);
            // Sub alpha*matrix[start_row] from every matrix[i]:
            for (size_t i = start_row + 1; i < ROWS_N; i++) {
                ElementType alpha = matrix_(i, max_abs_j) / matrix_(start_row, max_abs_j);
                if (alpha != 0) {
                    for (size_t j = 0; j < COLUMNS_N; j++) {
                        matrix_(i, j) = matrix_(i, j) - (alpha * matrix_(start_row, j));
                        if (new_max_abs < std::abs(matrix_(i, j))) {
                            new_max_i = i;
                            new_max_j = j;
                            new_max_abs = std::abs(matrix_(i, j));
                        }
                    }
                    vect_(i) = vect_(i) - alpha * vect_(start_row);
                }
            }
            ASSERT(new_max_i > start_row);
            ASSERT(new_max_j != max_abs_j);
            LOG_MATRIX(DEBUG, "Matrix after sub:\n" << matrix_);
            return std::make_pair(new_max_i, new_max_j);
        }


        Vector<ROWS_N, ElementType> GetAnsVect()
        {
            Vector<ROWS_N, ElementType> ans;
            for (size_t i = 0; i < ROWS_N; i++) {
                ans(ans_map_(i)) = vect_(i) / matrix_(i, ans_map_(i));
            }
            return ans;
        }

    private:
        void SwapRows(size_t i, size_t j)
        {
            ASSERT(i != j);
            matrix_.SwapRows(i, j);
            vect_.SwapElemets(i, j);
        }

    private:
        Matrix<ROWS_N, COLUMNS_N, MatrixType::ROW_BASED, ElementType> matrix_;
        Vector<ROWS_N, ElementType> vect_;
        
        // Vector that is initialized and used during BackwardElimination;
        // Filled in orderd from [COLUMNS_N - 1] to [0];
        // [i]-elem stores an index with a non-zero element in a matrix of i-th row.
        Vector<COLUMNS_N, size_t> ans_map_;
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
            out << matrix(i, j) << " ";
        }
        out << std::endl;
    }
    return out;
}

}  // cmath

#endif  // LIB_MATRIX_H_INCLUDED
