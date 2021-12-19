#include "lib/interpolation.h"
#include <vector>
#include <iostream>
#include <sstream>

using namespace cmath;

std::vector<field_t> X_TBL = {
    0.2,
    0.25,
    0.27,
    0.3
};

std::vector<field_t> Y_TBL = {
    0.029,
    -0.080,
    -0.122,
    -0.185
};

int main() {
    Interpolator comp(Y_TBL.data(), X_TBL.data(), X_TBL.size());

    std::cout << "-----------------------------\n";
    std::cout << "f(x) = (x - 1)^2 - 0.5 e^x:\n";
    std::cout << "Interpolation:\n";
    std::cout << comp.GetLagrangeString() << std::endl;
    std::cout << "f(x) = 0 solution:\n";
    std::cout.precision(3);
    // Err ~ 0.029 * 0.080 * 0.122 * 0.185 * 2 exp(0.185)
    std::cout << "x = " << comp.GetLagrange().Eval(0) << ", Delta ~ 0.001" << std::endl;

    return 0;
}
