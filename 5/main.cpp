#include "lib/numeric_integration.h"
#include <vector>
#include <iostream>
#include <sstream>

using namespace cmath;

// VII.9.5(b):
std::vector<field_t> X_TBL = {
    0.,
    0.125,
    0.25,
    0.375,
    0.5,
    0.625,
    0.75,
    0.875,
    1.
};
std::vector<field_t> Y_TBL = {
    0.000000,
    0.021470,
    0.293050,
    0.494105,
    0.541341,
    0.516855,
    0.468617,
    0.416531,
    0.367879
};

int main() {
    Integrator comp(X_TBL.data(), Y_TBL.data(), X_TBL.size());

    size_t step = 0;

    step = 1;
    std::cout << "-----------------------------\n";
    std::cout << "Trapezoidal rule (grid multiplier = " << step << "):" << std::endl;
    field_t tr_1 = comp.GetTrapezoid(step);
    std::cout << " I1 = " << tr_1 << std::endl;

    step = 2;
    std::cout << "-----------------------------\n";
    std::cout << "Trapezoidal rule (grid multiplier = " << step << "):" << std::endl;
    field_t tr_2 = comp.GetTrapezoid(step);
    std::cout << " I2 = " << tr_2 << std::endl;

    std::cout << "-----------------------------\n";
    std::cout << "Richardson extrapolation:" << std::endl;
    field_t h = comp.GetH();
    field_t chp = (tr_1 - tr_2) / (4 - 1);
    std::cout << " delta = " << chp << std::endl;
    std::cout << " I = " << tr_1 + chp << std::endl;

    std::cout << "-----------------------------\n";
    std::cout << "Simpson's rule:" << std::endl;
    field_t simpson = comp.GetSimpson();
    std::cout << " Is = " << simpson << std::endl;

    return 0;
}
