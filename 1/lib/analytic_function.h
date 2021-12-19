#ifndef ANALYTIC_FUNCTION_H_INCLUDDED
#define ANALYTIC_FUNCTION_H_INCLUDDED

#include "log.h"
#include "macro.h"

#include <string>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <functional>

namespace cmath {

#define FAILURE(msg) {  \
    LOG(ERROR, msg);    \
    failed_ = true;     \
}

const size_t MAX_OPS = 10u;


using field_t = double;


class AFunction {
public:
    enum class Element {
        #define def(name, impl, token, derivative) name,
        #include "elem_func_def-include.h"
        #undef def
    };

    enum NumDifMethods {
        FIRST = 1,
        SECOND,
        THIRD,
        FOURTH,
        FIFTH
    };

    class Parse {
    public: 
        Parse(const std::string &string, AFunction &main_func) {
            FormatString(string);
            formula_ = c_str_;
            char ch = GetNullChar(main_func);
            if (failed_) {
                LOG(ERROR, "UNKNOWN CHARACTER: " << ch);
                AFunction garbage = main_func;
                main_func = AFunction{};
            }
        }

        ~Parse() {
            delete [] c_str_;
            c_str_ = nullptr;
        }
        
        void FormatString(const std::string &string) {
            c_str_      = new char [string.size() + 1]();
            size_t idx  = 0;
            for (auto ch : string) {
                if (!IsSpace(ch)){
                    c_str_[idx++] = ch;
                }
            }
            return;
        }

        bool IsSpace(char ch) {
            switch(ch) {
                case ' '  : return true;
                case '\t' : return true;
                case '\n' : return true;
                default   : return false;
            }
        }

        char GetNullChar(AFunction &func);
        AFunction GetSum();
        AFunction GetSumTerm(char sign = '+');
        AFunction GetMulTerm(bool is_divisor = false);
        bool GetBrackets(AFunction &func);

    private:
        std::string formula_{""};
        char        *c_str_{nullptr};
        size_t      idx_{0};
        bool        failed_{false};
    };

public:
    AFunction() = default;
    AFunction(AFunction&& src);
    AFunction(const AFunction& src);
    AFunction& operator=(AFunction&& src);
    AFunction& operator=(const AFunction& src);
    AFunction(std::string formula) {
        Parse(formula, *this);
    }
    ~AFunction();


    field_t     Eval(field_t x, field_t param = 0) const;
    AFunction   Differentiate() const;
    field_t     EvalNumDerivative(field_t x, field_t step, size_t method_num) const;
    void        SearchAndReplaceArgumentIn(AFunction& func) const;
    bool        Optimize();

    AFunction*  AllocateOperands(size_t n);
    bool        IsOperandOf(const AFunction &other) const;
    bool        IsConst() const;
    bool        IsValid() const;
    void        SetType(Element type)
    {
        LOG(DEBUG, "setting type" << (int) type);
        type_ = type;
    }

    /**
     * Implicit casts of Element::CONST to number:
     */
    AFunction(field_t val)
    {
        type_ = Element::CONST;
        num_ = val;
    }

    operator field_t() const
    {
        ASSERT(type_ == Element::CONST);
        return num_;
    }
    field_t operator=(field_t val)
    {
        delete [] operands_;
        operands_ = nullptr;
        noperands_ = 0;
        type_ = Element::CONST;
        return num_ = val;
    }

    field_t GetNum() const
    {
        return num_;
    }

    void Dump(std::ostream *out);
    void DumpOperands(std::ostream *out, char binop);

    friend std::ostream& operator<<(std::ostream &ou, cmath::AFunction &func);
    friend std::ostream& operator<<(std::ostream &ou, cmath::AFunction::Element type);

private:
    // mul or sum terms or function argument:
    AFunction       *operands_{nullptr};
    size_t          noperands_{0};
    Element         type_{Element::INVTOK};
    field_t         num_{0};
    
};

} // namespace cmath

#endif // ANALYTIC_FUNCTION_H_INCLUDDED
