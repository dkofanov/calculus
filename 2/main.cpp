#include "lib/matrix.h"
#include "../1/lib/analytic_function.h"
#include <iostream>
#include <sstream>

using namespace cmath;

int main() {
    constexpr size_t n = 3;
    Matrix<n, n, MatrixType::ROW_BASED, AFunction> m;
    for (size_t i = 0; i < n; i++) {
        m(i, 1) = AFunction("3.14");
    }
    for (size_t i = 0; i < n; i++) {
        std::stringstream formula;
        formula << m(i, 1).GetNum() << " - x";
        m(i, 2) = AFunction(formula.str());
    }

    Vector<n, AFunction> v;
    for (size_t i = 0; i < n; i++) {
        v(i) = AFunction("22.8");
    }
    m.SolveGauss(v);
    return 0;
}
