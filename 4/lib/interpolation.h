#ifndef NUMERICAL_INTEGRATION_H_INCLUDDED
#define NUMERICAL_INTEGRATION_H_INCLUDDED

#include "../../1/lib/analytic_function.h"
#include "../../1/lib/log.h"
#include "../../1/lib/macro.h"

#include <string>
#include <sstream>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <functional>

namespace cmath {

class Interpolator {
public:
    Interpolator(const field_t *x_table, const field_t *y_table, size_t table_size)
    : x_tbl_(x_table)
    , y_tbl_(y_table)
    , tbl_size_(table_size) {}

    AFunction GetLagrange();
    std::string GetLagrangeString();

private:
    std::string GetLagrangeTermString(size_t i);

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

    const field_t *x_tbl_{nullptr};
    const field_t *y_tbl_{nullptr};
    size_t tbl_size_{0};
    field_t h_{0};
};

AFunction Interpolator::GetLagrange() {
    ASSERT(NullCheck());
    ASSERT(tbl_size_ >= 2);

    std::string poly_str = GetLagrangeString();
    return AFunction(poly_str);
}

    
std::string Interpolator::GetLagrangeString() {
    std::string poly = "";
    for (size_t i = 0; i < tbl_size_ - 1; i++) {
        poly += GetLagrangeTermString(i) + " + ";
    }
    poly += GetLagrangeTermString(tbl_size_ - 1);

    return poly;
}

std::string Interpolator::GetLagrangeTermString(size_t i) {
    std::stringstream term;
    term << y_tbl_[i] << " * ";
    for (size_t j = 0; j < i; j++) {
        term << "(x - (" << x_tbl_[j] << ")) / ";
        term << "((" << x_tbl_[i] << ") - ("  << x_tbl_[j] << "))";
        if (!((i == tbl_size_ - 1) && (j == i - 1))) {
            term  << " * ";
        }
    }
    for (size_t j = i + 1; j < tbl_size_; j++) {
        term << "(x - (" << x_tbl_[j] << ")) / ";
        term << "((" << x_tbl_[i] << ") - ("  << x_tbl_[j] << "))";
        if (j != tbl_size_ - 1) {
            term  << " * ";
        }
    }
    return term.str();
}

}  // namespace cmath

#endif  // NUMERICAL_INTEGRATION_H_INCLUDDED
