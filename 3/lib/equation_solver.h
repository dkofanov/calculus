#ifndef EQUATION_SOLVER_H_INCLUDDED
#define EQUATION_SOLVER_H_INCLUDDED

#include "../../1/lib/analytic_function.h"
#include "../../1/lib/log.h"
#include "../../1/lib/macro.h"

#include <cmath>
#include <cstdlib>

namespace cmath {

class EquationSolver {
public:
    EquationSolver(const AFunction &normal_eq)
    : equation_(normal_eq) {}

    virtual field_t Solve() = 0;

private:
    AFunction equation_{};
};

class SimpleIterationsSolver : EquationSolver {
public:
    SimpleIterationsSolver(const AFunction &normal_eq, const AFunction &contractor, 
                     field_t min, field_t max, field_t epsilon = 0.001)
    : EquationSolver(normal_eq)
    , contractor_(contractor)
    , min_(min)
    , max_(max) 
    , epsilon_(epsilon) {}

    virtual field_t Solve() override;
    size_t GetItersN() {
        return iters_n_;
    }

private:
    AFunction contractor_{};
    size_t iters_n_{0};
    field_t min_{0};
    field_t max_{0};
    field_t epsilon_{0};
};

field_t SimpleIterationsSolver::Solve() {
    AFunction contraction = contractor_.Differentiate();

    ASSERT(std::abs(contraction.Eval((min_ + max_) / 2)) < 1.);
    field_t x_cur = contractor_.Eval((min_ + max_) / 2);

    ASSERT(std::abs(contraction.Eval(x_cur)) < 1.);
    field_t x_next = contractor_.Eval(x_cur);
    iters_n_ = 1;
    while (std::abs(x_cur - x_next) > epsilon_) {
        iters_n_++;
        x_cur = x_next;
        ASSERT(std::abs(contraction.Eval(x_cur)) < 1.);
        x_next = contractor_.Eval(x_cur);
    }
    return x_next;
}

}  // namespace cmath

#endif  // EQUATION_SOLVER_H_INCLUDDED
