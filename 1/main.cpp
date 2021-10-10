#include "lib/analytic_function.h"

#include <vector>
#include <fstream>

using namespace CMath;

int main()
{
    std::vector<AFunction> functions(5);
    functions[0] = AFunction("sin(pow(x,2))");
    functions[1] = AFunction("cos(sin(x))");
    functions[2] = AFunction("exp(sin(cos(x)))");
    functions[3] = AFunction("ln(x + 3)");
    functions[4] = AFunction("pow(x + 3,0.5)");

    std::vector<std::ofstream> out(5);
    out[0].open("sinx2.dat");
    out[1].open("cossinx.dat");
    out[2].open("expsincosx.dat");
    out[3].open("lnx3.dat");
    out[4].open("sqrtx3.dat");

    field_t h = 2.;
    field_t x0 = 5.;
    for (size_t i = 0; i < 21; i++) {
        h /= 2;
        for (size_t j = 0; j < 5; j++) {
            field_t analytic_val = functions[j].Differentiate().Eval(x0);
            out[j] << std::scientific << h;
            for (size_t k = 1; k < 6; k++) {
                field_t numeric_val = functions[j].EvalNumDerivative(x0, h, k);
                field_t delta = std::abs(analytic_val - numeric_val);
                out[j] << '\t' << delta;
            }
            out[j] << '\n';
        }
    }

    for (auto &f : out) {
        f.close();
    }
    return 0;
}