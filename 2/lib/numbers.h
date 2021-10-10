#ifndef LIB_NUMBERS_H_INCLUDED
#define LIB_NUMBERS_H_INCLUDED

namespace cmath {

template <typename NUMBER_TYPE>
class Number
{
public:
    Number(NUMBER_TYPE val) : val_(val) {}
    operator NUMBER_TYPE() const
    {
        return val_;
    }
private:
    NUMBER_TYPE val_{0};
};

using RealNumber = Number<double>;

}  // cmath

#endif  // LIB_NUMBERS_H_INCLUDED
