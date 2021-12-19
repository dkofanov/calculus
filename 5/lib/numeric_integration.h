#ifndef NUMERICAL_INTEGRATION_H_INCLUDDED
#define NUMERICAL_INTEGRATION_H_INCLUDDED

#include "../../1/lib/analytic_function.h"
#include "../../1/lib/log.h"
#include "../../1/lib/macro.h"

#include <string>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <functional>

namespace cmath {

class Integrator {
public:
    Integrator(const field_t *x_table, const field_t *y_table, size_t table_size)
    : x_tbl_(x_table)
    , y_tbl_(y_table)
    , tbl_size_(table_size) {}

    field_t GetTrapezoid(size_t step_multiplier);
    field_t GetRichardsonExtrapolationDifference(size_t );
    field_t GetSimpson();

    field_t GetH() {
        return h_;
    }

private:
    bool NullCheck() {
        if (x_tbl_ == nullptr) {
            return false;
        }
        if (y_tbl_ == nullptr) {
            return false;
        }
        return true;
    }
    bool IsUniformMesh() {
        ASSERT(x_tbl_ != nullptr);
        ASSERT(tbl_size_ >= 2);

        field_t h_0 = x_tbl_[1] - x_tbl_[0];

        for (size_t i = 1; i < tbl_size_ - 1; i++) {
            field_t h_i = x_tbl_[1] - x_tbl_[0];
            if (h_i != h_0) {
                return false;
            }
        }

        h_ = h_0;
        return true;
    }

    const field_t *x_tbl_{nullptr};
    const field_t *y_tbl_{nullptr};
    size_t tbl_size_{0};
    field_t h_{0};
};

field_t Integrator::GetTrapezoid(size_t step_multiplier) {
    ASSERT(NullCheck());
    ASSERT(tbl_size_ >= 2);

    ASSERT((tbl_size_ - 1) % step_multiplier == 0);

    if (IsUniformMesh()) {
        field_t sum = 0;

        sum += y_tbl_[0] / 2;

        sum += y_tbl_[tbl_size_ - 1] / 2;
            
        for (size_t i = step_multiplier; i < tbl_size_ - 1; i += step_multiplier) {
            sum += y_tbl_[i];
        }
        return sum * h_ * step_multiplier;
    } else {
        UNREACHABLE();
    }
}

field_t Integrator::GetSimpson() {
    ASSERT(NullCheck());
    ASSERT(tbl_size_ >= 3);

    if (IsUniformMesh()) {
        field_t sum0 = 0;

        sum0 += y_tbl_[0];
        sum0 += y_tbl_[tbl_size_ - 1];

        field_t sum1 = 0;
        for (size_t i = 1; i < tbl_size_ - 1; i += 2) {
            sum1 += y_tbl_[i];
        }

        field_t sum2 = 0;
        for (size_t i = 2; i < tbl_size_ - 1; i += 2) {
            sum2 += y_tbl_[i];
        }

        return (sum0 + 4 * sum1 + 2 * sum2) * h_ / 3;
    } else {
        UNREACHABLE();
    }
}

}  // namespace cmath

#endif  // NUMERICAL_INTEGRATION_H_INCLUDDED
