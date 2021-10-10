//      name,   implementation, token,  derivative
def(    EXP,    std::exp,       exp,    exp(x))
def(    LN,     std::log,       ln,     1/x)
def(    SIN,    std::sin,       sin,    cos(x))
def(    COS,    std::cos,       cos,    -sin(x))
def(    TG,     std::tan,       tg,     1/pow(cos(x), 2))
def(    ASIN,   std::asin,      asin,   pow(1-pow(x, 2), -0.5))
def(    ACOS,   std::acos,      acos,   pow(1-pow(x, 2), -0.5))
def(    ATG,    std::atan,      atan,   pow(1+pow(x, 2), -1))