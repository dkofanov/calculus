#include "lib/matrix.h"
#include "../1/lib/analytic_function.h"
#include <iostream>
#include <sstream>

using namespace cmath;

int main() {
    constexpr size_t n = 3;
    Matrix<n, n, MatrixType::ROW_BASED, AFunction> m;
    m(0, 0) = 2;    m(0, 1) = -1;   m(0, 2) = 0;
    m(1, 0) = -1;   m(1, 1) = 1;    m(1, 2) = 4;
    m(2, 0) = 1;    m(2, 1) = 2;    m(2, 2) = 3;
    Vector<n, AFunction> v;
        v(0) = AFunction("0");
        v(1) = AFunction("13");
        v(2) = AFunction("14");
    std::cout << m.SolveGauss(v) <<std::endl;
    return 0;
}
