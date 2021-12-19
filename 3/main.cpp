#include "lib/equation_solver.h"
#include <vector>
#include <iostream>
#include <sstream>

#define NDEBUG

using namespace cmath;

// IV.12.8. (b)

int main() {
    field_t epsilon = 0.001;
    AFunction norm_eq("x * exp(-pow(x,2)) - pow(2,-0.5) * exp(-0.5)");
    AFunction contractor_1("pow(8,-0.5) * exp(pow(x,2) - 0.5)");
    field_t min_1 = 0;
    field_t max_1 = 0.71;

    AFunction contractor_2("pow(ln(x * pow(8, 0.5) * exp(0.5)), 0.5)");
    field_t min_2 = 0.71;
    field_t max_2 = 2;

    SimpleIterationsSolver solver_1(norm_eq, contractor_1, min_1, max_1, epsilon);
    SimpleIterationsSolver solver_2(norm_eq, contractor_2, min_2, max_2, epsilon);
    
    field_t x1 = solver_1.Solve();
    field_t x2 = solver_2.Solve();

    std::cout << "-----------------------------\n";
    std::cout << "Full width at half maximum:\n";
    std::cout.precision(3);
    std::cout << std::fixed << "x1 = " << x1 << std::endl;
    std::cout << "x2 = " << x2 << std::endl;
    std::cout << "dx = " << x2 - x1 << std::endl;
    return 0;
}
