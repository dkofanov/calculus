#ifndef LIB_MATRIX_H_INCLUDED
#define LIB_MATRIX_H_INCLUDED

#include <cstddef>
#include <iostream>
#include <utility>
#include <iterator>
#include "../../1/lib/macro.h"
#include "../../1/lib/log.h"

namespace cmath {

enum class NormType {
    MAX_ABS,
    SUM_ABS,
    EUCLIDIAN
};


template <size_t N, typename ElementType>
class Vector {
public:
    using VectorT = Vector<N, ElementType>;

    Vector()
    {
        data_ = new ElementType[N];
        ASSERT(data_ != nullptr);
    }

    Vector(const VectorT &vect)
    {
        data_ = new ElementType[N];
        ASSERT(data_ != nullptr);
        for (size_t i = 0; i < N; i++) {
            data_[i] = vect(i);
        }
    }
    Vector(VectorT &&vect)
    {
        data_ = vect.data_;
        vect.data_ = nullptr;
    }
    auto operator=(VectorT &&vect)
    {
        data_ = vect.data_;
        vect.data_ = nullptr;
        return *this;
    }

    void Initialize(const std::initializer_list<ElementType> l)
    {
        ASSERT(l.size() == N);
        ASSERT(data_ != nullptr);
        size_t i = 0; 
        for (const auto &el : l) {
            data_[i] = el;
            i++;
        }
    }

    ~Vector() {
        LOG_MATRIX(DEBUG, "vecdtor(" << this << "):");
        delete [] data_;
        data_ = nullptr;
    }

    auto operator-(const VectorT &rhs) const
    {
        VectorT res;
        for (size_t i = 0; i < N; i++) {
            res.data_[i] = data_[i] - rhs.data_[i];
        }
        return res;
    }

    auto operator+(const VectorT &rhs) const
    {
        VectorT res;
        for (size_t i = 0; i < N; i++) {
            res.data_[i] = data_[i] + rhs.data_[i];
        }
        return res;
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
    
    bool IsConst() const
    {
        for (size_t i = 0; i < N; i++) {
            if (!data_[i].IsConst()) {
                return false;
            }
        }
        return true;
    }

    void SwapElemets(size_t i, size_t j)
    {
        ASSERT(data_ != nullptr);
        ASSERT(i < N);
        ASSERT(j < N);
        ASSERT(data_ != nullptr);

        std::swap(data_[i], data_[j]);
    }

    template <NormType type>
    ElementType Norm() {
        switch (type)
        {
        case NormType::MAX_ABS:
            return NormMaxAbs();
        
        default:
            UNREACHABLE();
        }
    }

    ElementType NormMaxAbs() {
        ASSERT(data_ != nullptr);
        ElementType max_abs = 0;
        for (size_t i = 0; i < N; i++) {
            if (max_abs < std::abs(data_[i])) {
                max_abs = std::abs(data_[i]);
            }
        }
        return max_abs;
    }

private:
    ElementType *data_{nullptr};
    template <size_t N_ELEM, typename ElementT>
    friend std::ostream& operator<<(std::ostream &ou, const Vector<N_ELEM, ElementT> &v);
};

template <size_t N_ELEM, typename ElementT>
std::ostream& operator<<(std::ostream &out, const Vector<N_ELEM, ElementT> &v)
{
    for (size_t i = 0; i < N_ELEM - 1; i++) {
        out << v.data_[i] << '\n';
    }
    out << v.data_[N_ELEM - 1];

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
    using MatrixT = Matrix<ROWS_N, COLUMNS_N, MatrixType::ROW_BASED, ElementType>;
    using TrMatrixT = Matrix<COLUMNS_N, ROWS_N, MatrixType::ROW_BASED, ElementType>;
    using VectorT = Vector<ROWS_N, ElementType>;

    enum class Op {
        SWAP_ROWS,
        ADD_ROW_TO
    };
    Matrix()
    {
        data_ = new ElementType *[ROWS_N];
        ASSERT(data_ != nullptr);
        for (size_t i = 0; i < ROWS_N; i++) {
            data_[i] = new ElementType[COLUMNS_N];
            ASSERT(data_[i] != nullptr);
        }
    }
    Matrix(const MatrixT &matrix)
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
        ASSERT(IsConst());
    }
    ~Matrix()
    {
        LOG_MATRIX(DEBUG, "matdtor(" << this << "):");
        ASSERT(IsConst());
        ASSERT(data_ != nullptr);
        for (size_t i = 0; i < ROWS_N; i++) {
            delete [] data_[i];
        }
        delete [] data_;
        data_ = nullptr;
    }
    
    constexpr size_t GetRowsN() const
    {
        return ROWS_N;
    }

    constexpr size_t GetColumnsN() const
    {
        return COLUMNS_N;
    }

    void SwapRows(size_t i, size_t j)
    {
        std::swap(data_[i], data_[j]);
    }

    auto FindMaxAbsElement()
    {
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

    bool IsConst() const
    {
        for (size_t i = 0; i < ROWS_N; i++) {
            for (size_t j = 0; j < COLUMNS_N; j++) {
                if (!data_[i][j].IsConst()) {
                    return false;
                }
            }
        }
        return true;
    }

    auto SolveGauss(const Vector<ROWS_N, ElementType> &vect) const
    {
        auto sol = Gauss(*this, vect);
        return sol;
    }
    template <size_t MAX_ITERS>
    auto SolveIterations(const Vector<ROWS_N, ElementType> &vect, const ElementType &target_error) const
    {
        auto sol = SimpleIterations<MAX_ITERS>(*this, vect, target_error);
        return sol;
    }
    void Initialize(const std::initializer_list<ElementType> l)
    {
        ASSERT(l.size() == ROWS_N * COLUMNS_N);
        size_t i = 0; 
        for (const auto &el : l) {
            data_[i / COLUMNS_N][i % COLUMNS_N] = el;
            i++;
        }
    }

    void Neutralize()
    {
        ASSERT(ROWS_N == COLUMNS_N);
        for (size_t i = 0; i < ROWS_N; i++) {
            for (size_t j = 0; j < COLUMNS_N; j++) {
                if (i == j) {
                    data_[i][j] = 1;
                } else {
                    data_[i][j] = 0;
                }
            }
        }
    }

    void Zero()
    {
        ASSERT(ROWS_N == COLUMNS_N);
        for (size_t i = 0; i < ROWS_N; i++) {
            for (size_t j = 0; j < COLUMNS_N; j++) {
                data_[i][j] = 0;
            }
        }
    }

    bool operator==(const MatrixT &rhs) const
    {
        constexpr double EPSILON = 0.00000001;
        for (size_t i = 0; i < ROWS_N; i++) {
            for (size_t j = 0; j < COLUMNS_N; j++) {
                if (std::abs(data_[i][j]- rhs.data_[i][j]) > EPSILON) {
                    return false;
                }
            }
        }
        return true;
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

    auto operator*(const TrMatrixT &rhs) const
    {
        Matrix<ROWS_N, ROWS_N, MatrixType::ROW_BASED, ElementType> prod;
        prod.Zero();
        for (size_t i = 0; i < ROWS_N; i++) {
            for (size_t j = 0; j < COLUMNS_N; j++) {
                for (size_t k = 0; k < ROWS_N; k++) {
                    prod(i, k) = prod(i, k) + (data_[i][j] * rhs.data_[j][k]);
                }
            }
        }
        return prod;
    }

    auto operator*(const Vector<COLUMNS_N, ElementType> &rhs) const
    {
        Vector<ROWS_N, ElementType> prod;
        for (size_t i = 0; i < ROWS_N; i++) {
            prod(i) = 0;
            for (size_t j = 0; j < COLUMNS_N; j++) {
                prod(i) = prod(i) + (data_[i][j] * rhs(j));
            }
            ASSERT(prod(i).IsConst());
        }
        return prod;
    }
    void operator-=(const MatrixT &rhs)
    {
        for (size_t i = 0; i < ROWS_N; i++) {
            for (size_t j = 0; j < COLUMNS_N; j++) {
                data_[i][j] = data_[i][j] - rhs.data_[i][j];
            }
        }
    }
    
    template <typename T, NormType type>
    ElementType Norm()
    {
        switch (type)
        {
        case NormType::MAX_ABS:
            return NormMaxAbs();
        
        default:
            UNREACHABLE();
        }
    }

    ElementType NormMaxAbs()
    {
        ElementType norm = 0;
        for (size_t i = 0; i < ROWS_N; i++) {
            ElementType i_row_sum = 0;
            for (size_t j = 0; j < COLUMNS_N; j++) {
                i_row_sum = i_row_sum + std::abs(data_[i][j]);
            }
            if (norm < i_row_sum) {
                norm = i_row_sum;
            }
        }
        return norm;
    }

private:
    class Gauss {
    public:
        Gauss(const MatrixT &matrix,
              const Vector<ROWS_N, ElementType> &vect) : matrix_(matrix), vect_(vect), orig_(matrix) {
            ASSERT(matrix_.GetRowsN() == matrix_.GetColumnsN());
            inv_matrix_.Neutralize();
            ForwardElimination();
            BackwardElimination();
        }

        void ForwardElimination() {
            auto [i, j] = matrix_.FindMaxAbsElement();
            LOG_MATRIX(DEBUG, "Start ForwardElimination():\nmatrix:\n" << matrix_ << inv_matrix_ <<
                              "vector:\n" << vect_ << '\n');
            ForwardStepRec(0, i, j);
            LOG_MATRIX(DEBUG, "End ForwardElimination():\nmatrix:\n" << matrix_ << inv_matrix_ <<
                              "vector:\n" << vect_ << '\n');                              
        }

        void CheckInv() {
            auto m = inv_matrix_ * orig_;
            LOG_MATRIX(DEBUG, "inv :\n" << inv_matrix_);
            LOG_MATRIX(DEBUG, "*");
            LOG_MATRIX(DEBUG, "orig:\n" << orig_);
            LOG_MATRIX(DEBUG, "prod:\n" << m);
            LOG_MATRIX(DEBUG, "cur :\n" << matrix_);
            ASSERT(m == matrix_);
        }
        void ForwardStepRec(size_t start_row_idx, size_t max_abs_i, size_t max_abs_j)
        {
            // Move the "based" row above in the matrix:
            if (start_row_idx != max_abs_i) {
                SwapRows(start_row_idx, max_abs_i);
                CheckInv();
            }
            // Normalize the "based" row:
            auto [new_max_i, new_max_j] = SubBasedRow(start_row_idx, max_abs_j);
            if (start_row_idx != (ROWS_N - 2)) {
                ForwardStepRec(start_row_idx + 1, new_max_i, new_max_j);
                CheckInv();
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
                    for (size_t k = 0; k < COLUMNS_N; k++) {
                        inv_matrix_(i, k) = inv_matrix_(i, k) - (alpha * inv_matrix_(j, k));
                    }
                    CheckInv();
                    LOG_MATRIX(DEBUG, "inv:\n" << inv_matrix_);
                    vect_(i) = vect_(i) - alpha * vect_(j);
                }
            }
            ans_map_(0) = FindNonZeroElement(static_cast<size_t>(0));

            LOG_MATRIX(DEBUG, "End BackwardElimination():\nmatrix:\n" << matrix_ <<
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
            LOG_MATRIX(DEBUG, "Matrix before sub:\n" << matrix_ << inv_matrix_);
            // Sub alpha*matrix[start_row] from every matrix[i]:
            for (size_t i = start_row + 1; i < ROWS_N; i++) {
                ElementType alpha = matrix_(i, max_abs_j) / matrix_(start_row, max_abs_j);
                for (size_t j = 0; j < COLUMNS_N; j++) {
                    matrix_(i, j) = matrix_(i, j) - (alpha * matrix_(start_row, j));
                    inv_matrix_(i, j) = inv_matrix_(i, j) - (alpha * inv_matrix_(start_row, j));
                    if (new_max_abs < std::abs(matrix_(i, j))) {
                        new_max_i = i;
                        new_max_j = j;
                        new_max_abs = std::abs(matrix_(i, j));
                    }
                }
                vect_(i) = vect_(i) - alpha * vect_(start_row);
            }
            ASSERT(new_max_i > start_row);
            ASSERT(new_max_j != max_abs_j);
            LOG_MATRIX(DEBUG, "Matrix after sub:\n" << matrix_ << inv_matrix_);
            return std::make_pair(new_max_i, new_max_j);
        }

        Vector<ROWS_N, ElementType> GetAnsVector()
        {
            Vector<ROWS_N, ElementType> ans;
            auto inv_copy = inv_matrix_;
            for (size_t i = 0; i < ROWS_N; i++) {
                ans(ans_map_(i)) = vect_(i) / matrix_(i, ans_map_(i));
                for (size_t j = 0; j < ROWS_N; j++) {
                    inv_matrix_(ans_map_(i), j) = inv_copy(i, j) / matrix_(i, ans_map_(i));
                } 
            }
            return ans;
        }

        auto GetInvMatrix() {
            return inv_matrix_;
        }

    private:
        void SwapRows(size_t i, size_t j)
        {
            ASSERT(i != j);
            matrix_.SwapRows(i, j);
            vect_.SwapElemets(i, j);
            inv_matrix_.SwapRows(i, j);
        }

    private:
        MatrixT matrix_;
        Vector<ROWS_N, ElementType> vect_;
        
        // Vector that is initialized and used during BackwardElimination;
        // Filled in orderd from [COLUMNS_N - 1] to [0];
        // [i]-elem stores an index with a non-zero element in a matrix of i-th row.
        Vector<COLUMNS_N, size_t> ans_map_;

        MatrixT inv_matrix_;
        const MatrixT &orig_;
    };

    template <size_t MAX_ITER_N>
    class SimpleIterations
    {
    public:
        SimpleIterations(const MatrixT &matrix, const Vector<ROWS_N, ElementType> &vect,
                         const ElementType &target_error) : f_vect_(vect), orig_(matrix) {
            ASSERT(matrix.GetRowsN() == matrix.GetColumnsN());
            b_matrix_.Neutralize();
            b_matrix_ -= matrix;
            LOG_MATRIX(DEBUG, "`b_matrix_`:\n" << b_matrix_);
            for (size_t i = 0; i < COLUMNS_N; i++) {
                u_vect_(i) = 0;
            }
            num_iterations_ = Iterate(target_error);
        }

        size_t Iterate(const ElementType &target_error)
        {
            size_t i = 0;
            LOG_MATRIX(DEBUG, "`b_matrix_` norm: " << b_matrix_.NormMaxAbs());
            //ASSERT(b_matrix_.NormMaxAbs() < 1);
            for (i = 0; i < MAX_ITER_N; i++) {
                i++;
                auto u1 = (b_matrix_ * u_vect_) + f_vect_;
                auto diff = (u1 - u_vect_);
                if (diff.NormMaxAbs() < target_error) {
                    LOG_MATRIX(INFO, "Reached target_error, stopping iterations;");
                    return i;
                }
                u_vect_ = std::move(u1);
            }
            LOG_MATRIX(INFO, "Reached `MAX_ITER_N`, stopping iterations;");
            return i;
        }
        
        auto GetAnsVector() const
        {
            return u_vect_;
        }

        auto GetNumIterations() const
        {
            return num_iterations_;
        }

    private:
        MatrixT b_matrix_;
        Vector<ROWS_N, ElementType> f_vect_;
        Vector<COLUMNS_N, ElementType> u_vect_;
        // Vector<COLUMNS_N, ElementType> u_vect_prev_;
        size_t num_iterations_{0};
        
        const MatrixT &orig_;
    };

private:
    ElementType **data_{nullptr};

    template <size_t ROWS, size_t COLUMNS, typename ElementT>
    friend std::ostream& operator<<(std::ostream &ou, const Matrix<ROWS, COLUMNS, MatrixType::ROW_BASED, ElementT> &matrix);
};

template <size_t ROWS, size_t COLUMNS, typename ElementT>
std::ostream& operator<<(std::ostream &out, const Matrix<ROWS, COLUMNS, MatrixType::ROW_BASED, ElementT> &matrix)
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
