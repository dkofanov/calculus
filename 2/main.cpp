#include "lib/matrix.h"
#include "../1/lib/analytic_function.h"
#include <iostream>
#include <sstream>

using namespace cmath;

int main() {
    constexpr size_t n = 3;
    Matrix<n, n, MatrixType::COLUMN_BASED, AFunction> m;
    for (size_t i = 0; i < n; i++) {
        m(i, 1) = AFunction("3.14");
    }
    for (size_t i = 0; i < n; i++) {
        std::stringstream formula;
        formula << m(i, 1).GetNum() << " - x";
        m(i, 2) = AFunction(formula.str());
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            std::cout << m(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
