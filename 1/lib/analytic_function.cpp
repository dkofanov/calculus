#include "analytic_function.h"

namespace cmath {

char AFunction::Parse::GetNullChar(AFunction &func) {
    func = GetSum();
    if (c_str_[idx_] != '\0')
        failed_ = true;

    return c_str_[idx_];    
}



AFunction AFunction::Parse::GetSum() {
    LOG(DEBUG, "getting sum...");
    AFunction func{};
    bool can_eval = true;
    auto term0 = GetSumTerm();
    size_t i;
    for (i = 1; i < MAX_OPS; i++) {
        LOG(DEBUG, "looking for another addend...");
        switch (c_str_[idx_]) {
            case '+' :
                LOG(DEBUG, "+ sign found..");
                if (i == 1)
                    func.AllocateOperands(MAX_OPS);
                idx_++;
                func.operands_[i] = GetSumTerm();
                can_eval *= func.operands_[i].IsConst();
                break;
                
            case '-' :
                LOG(DEBUG, "- sign found..");
                if (i == 1)
                    func.AllocateOperands(MAX_OPS);
                idx_++;
                func.operands_[i] = GetSumTerm('-');
                can_eval *= func.operands_[i].IsConst();
                break;
                
            default :
            LOG(DEBUG, "loop breaked");
                goto loop_break;
        }
    }

loop_break:
    if (!term0.IsValid()) {
        ASSERT(func.operands_ != nullptr);
        // sum expression starts with '+' or '-';
        term0 = std::move(func.operands_[i - 1]);
        i--;
    }

    if (i == 1) {
        LOG(DEBUG, "convert sum of 1 function to the function itself");
        func = std::move(term0);
    } else {
        ASSERT(func.operands_ != nullptr);
        func.operands_[0] = std::move(term0);

        can_eval *= func.operands_[0].IsConst();
        if (can_eval) {
            LOG(DEBUG, "pre-evaluating sum..");
            AFunction temp{};
            temp.SetType(AFunction::Element::CONST);
            temp.num_ = func.Eval(0);
            func = std::move(temp);
        } else {
            func.noperands_ = i;
            func.SetType(AFunction::Element::SUM);
        }
    }
    LOG(DEBUG, "sum found");
    return func;
}



AFunction AFunction::Parse::GetSumTerm(char sign) {
    LOG(DEBUG, "getting addend...");
    AFunction temp{}, term{};
    term.SetType(AFunction::Element::MUL);
    term.num_ = 1;
    auto term0 = GetMulTerm();

    size_t i;
    for (i = 1; i < MAX_OPS; i++) {
        LOG(DEBUG, "looking for another mul term...");
        switch (c_str_[idx_]) {
            case '*' :
                LOG(DEBUG, " * sign found...");
                idx_++;
                temp = GetMulTerm();
                if (temp.IsConst())
                {
                    term.num_ *= temp.num_;
                    i--; // use the same location for the next iteration
                } else {
                    if (i == 1) {
                        term.AllocateOperands(MAX_OPS);
                    }
                    term.operands_[i] = std::move(temp);
                }
                break;
                
            case '/' :
                LOG(DEBUG, " / sign found...");
                idx_++;
                temp = GetMulTerm(true);
                if (temp.IsConst())
                {
                    if (temp.num_ == 0) {
                        LOG(ERROR, "ZERO DIV");
                        failed_ = true;
                        return AFunction{};
                    }
                    term.num_ /= temp.num_;
                    i--; // use the same location for the next iteration
                } else {
                    if (i == 1) {
                        term.AllocateOperands(MAX_OPS);
                    }
                    term.operands_[i] = std::move(temp);
                }
                break;

            default :
                // 'i' will specify the last used index + 1
                LOG(DEBUG, "loop breaked");
                goto loop_break;
        }
        LOG(DEBUG, "end looking for another mul term");
    }

loop_break:
    ASSERT(i <= MAX_OPS);
    if (sign == '-') {
        term.num_ *= -1;
    }

    if (term0.IsConst()) {
        term.num_ *= term0.num_;
        if (i == 1) {
            term.noperands_ = 0;
            ASSERT(term.operands_ == nullptr);
            term.SetType(AFunction::Element::CONST);
        } else {
            ASSERT(term.operands_ != nullptr);
            LOG(DEBUG, "placing last non-const f to operands_[0]:");
            LOG(DEBUG, "index: " << i - 1);
            term.operands_[0] = std::move(term.operands_[i - 1]);
            term.noperands_ = i - 1;
            LOG(DEBUG, "placed, type_ is: " << term.operands_[0].type_);
        }
    } else {
        if (i == 1 && term.num_ == 1) {
            ASSERT(term.operands_ == nullptr);
            term = std::move(term0);
        } else if (i == 1) {
            term.AllocateOperands(1);
            term.noperands_ = 1;
            term.operands_[0] = std::move(term0);
        } else {
            ASSERT(term.operands_ != nullptr);
            term.operands_[0] = std::move(term0);
            term.noperands_ = i;
        }
    }
    LOG(DEBUG, "sum term found");
    return term;
}


AFunction AFunction::Parse::GetMulTerm(bool is_divisor /* = false */) {
    LOG(DEBUG, "getting mul term...");
    AFunction term{};
    if (is_divisor) {
        term.SetType(AFunction::Element::POW);
        term.num_ = -1;
        term.AllocateOperands(1);
        term.noperands_ = 1;
        ASSERT(term.operands_ != nullptr);
        term.operands_[0] = GetMulTerm();
    } else if (!GetBrackets(term)) {
        LOG(DEBUG, "looking at: " << formula_.substr(idx_));
        if (c_str_[idx_] == '+' ||c_str_[idx_] == '-') {
            // num begining, should be parsed by GetSum;
            return AFunction{};
        }
        size_t idx2 = 0;
        double temp;
        try {
            temp = std::stod(formula_.substr(idx_), &idx2);
        } catch (std::invalid_argument) {}
        
        if (idx2 != 0) {
            LOG(DEBUG, "num taken : " << temp);
            term.num_ = temp;
            term.SetType(AFunction::Element::CONST);
            idx_ += idx2;
            return term;
        }

        if (c_str_[idx_] == 'x') {
            LOG(DEBUG, "found an argument");\
            idx_++;
            term.SetType(AFunction::Element::VAR);
            return term;
        }

        {
            std::string tkn("pow");
            size_t i = 0;
            for (auto c : tkn) {
                if (c == c_str_[idx_ + i]) {
                    i++;
                }
            }
            if (i == tkn.size()) {
                LOG(DEBUG, "token " << tkn << " parsed");
                idx_ += i;
                term.AllocateOperands(1);
                term.noperands_ = 1;
                ASSERT(c_str_[idx_] == '(');
                idx_++; //skip bracket;
                term.SetType(AFunction::Element::POW);
                term.operands_[0] = GetSum();
                ASSERT(c_str_[idx_] == ',');
                idx_++;
                auto temp = GetSum();
                ASSERT(temp.IsConst());
                ASSERT(c_str_[idx_] == ')');
                idx_++;
                term.num_ = temp.num_;
                return term;
            }                        
        }
        #define def(name, impl, token, derivative) { \
            std::string tkn(#token);            \
            size_t i = 0;                       \
            for (auto c : tkn) {                \
                if (c == c_str_[idx_ + i]) {    \
                    i++;                        \
                }                               \
            }                                   \
            if (i == tkn.size()) {              \
                LOG(DEBUG, "token " << tkn << " parsed");\
                idx_ += i;                      \
                term.AllocateOperands(1);       \
                term.noperands_ = 1;            \
                term.SetType(AFunction::Element::name);    \
                term.operands_[0] = GetMulTerm();  \
                return term;              \
            }                                   \
        }
        #include "elem_func_singleops-include.h"
        #undef def
        LOG(DEBUG, "no token found : " << formula_.substr(idx_));
        return AFunction{};
    }
    return term;
}

bool AFunction::Parse::GetBrackets(AFunction &func) {
    char ch = c_str_[idx_];
    size_t idx_sv = idx_;

    if (ch == '(') {
        LOG(DEBUG, "brackets opened");
        idx_++;
        func = GetSum();
        if (c_str_[idx_] != ')') {
            FAILURE("NO ENDING BRACKET AT: " << c_str_[idx_]);
            idx_ = idx_sv;
            return false;
        }
        idx_++;
        LOG(DEBUG, "brackets closed");
        return true;
    }

    return false;
}

AFunction::~AFunction() {
    LOG(DEBUG, "destructor call: " << (int) type_ );
    delete [] operands_;
    operands_ = nullptr;
    noperands_ = 0;
    type_ = AFunction::Element::INVTOK;
    num_ = 0;
}

AFunction* AFunction::AllocateOperands(size_t n) {
    ASSERT(!operands_);
    if (!operands_)
    {
        operands_ = new AFunction[n]{};
    }
    return operands_;
}

bool AFunction::IsConst() const {
    return type_ == AFunction::Element::CONST;
}

bool AFunction::IsValid() const {
    return type_ != AFunction::Element::INVTOK;
}

AFunction AFunction::Differentiate() const {
    AFunction ans{};
    switch(type_) {
        case AFunction::Element::SUM :
            ans.SetType(AFunction::Element::SUM);
            ans.AllocateOperands(noperands_);
            ASSERT(ans.operands_ != nullptr);
            ans.noperands_ = noperands_;
            for (size_t i = 0; i < noperands_; i++) {
                ans.operands_[i] = operands_[i].Differentiate();
            }
            ans.Optimize();
            return ans;

        case AFunction::Element::MUL :
            //ASSERT(noperands_ == 2);
            ans.SetType(AFunction::Element::SUM);
            ans.noperands_ = noperands_;
            ans.AllocateOperands(noperands_);
            for (size_t i = 0; i < noperands_; i++) {
                ans.operands_[i].SetType(AFunction::Element::MUL);
                ans.operands_[i].num_ = 1;
                ans.operands_[i].AllocateOperands(noperands_);
                ans.operands_[i].noperands_ = noperands_;
                ans.operands_[i] = operands_[i].Differentiate();
                for (size_t j = 0; j < noperands_; j++) {
                    if (j != i) {
                        ans.operands_[i].operands_[j] = operands_[j];
                    }
                }
            }
            ans.Optimize();
            return ans;

    #define def(name, impl, token, derivative)                          \
    case AFunction::Element::name :                                     \
            ASSERT(noperands_ == 1);                                    \
            LOG(DEBUG, "differeniating " << AFunction::Element::name);  \
            ans.SetType(AFunction::Element::MUL);                       \
            ans.num_ = 1;                                               \
            ans.AllocateOperands(2);                                    \
            ans.noperands_ = 2;                                         \
            ans.operands_[0] = AFunction(#derivative);                  \
            LOG(DEBUG, "fixing arg in\n" <<ans.operands_[0]);           \
            SearchAndReplaceArgumentIn(ans.operands_[0]);                   \
            ans.operands_[1] = operands_[0].Differentiate();             \
            LOG(DEBUG, "diff done:\n" << ans);                          \
            return ans;
                                        
    #include "elem_func_singleops-include.h"
    #undef def

        case AFunction::Element::POW :
                LOG(DEBUG, "differeniating pow");
                if (num_ == 1) {
                    ans.SetType(AFunction::Element::CONST);
                    ans.num_ = num_;
                } else if (num_ == 2) {
                    ans.SetType(AFunction::Element::MUL);
                    ans.num_ = 2;
                    ans.AllocateOperands(1);
                    ans.noperands_ = 1;
                    ans.operands_[0] = operands_[0];
                } else {
                    ans.SetType(AFunction::Element::MUL);
                    ans.num_ = num_;
                    ans.AllocateOperands(1);
                    ans.noperands_ = 1;
                    ans.operands_[0] = *this;
                    ans.operands_[0].num_ -= 1;
                }
                return ans;

        case AFunction::Element::CONST :
        case AFunction::Element::PARAM :
            ans.SetType(AFunction::Element::CONST);
            ans.num_ = 0;
            return ans;

        case AFunction::Element::VAR :
            ans.SetType(AFunction::Element::CONST);
            ans.num_ = 1;
            return ans;

        default : 
            UNREACHABLE();
    }
}

void AFunction::SearchAndReplaceArgumentIn(AFunction& func) const {
    ASSERT(noperands_ == 1);
    for (size_t i = 0; i < func.noperands_; i++) {
        if (func.operands_[i].type_ == AFunction::Element::VAR) {
            func.operands_[i] = operands_[0];
            LOG(DEBUG, "found arg; replaced with:\n" <<func.operands_[i]);
        } else {
            SearchAndReplaceArgumentIn(func.operands_[i]);
        }
    }

}

AFunction::AFunction(const AFunction &src) {
    LOG(DEBUG, "COPY CTOR");
    ASSERT(!src.IsOperandOf(*this));
    delete [] operands_;
    operands_ = nullptr;
    type_ = src.type_;
    num_ = src.num_;
    noperands_ = src.noperands_;
    AllocateOperands(MAX_OPS);
    for (size_t i = 0; i < src.noperands_; i++) {
        LOG(DEBUG, "copying operands_[" << i << "]...");
        operands_[i] = src.operands_[i];
    }
    return;
}

AFunction& AFunction::operator=(const AFunction &src) {
    LOG(DEBUG, "COPY ASSIGN");
    ASSERT(!src.IsOperandOf(*this));
    delete [] operands_;
    operands_ = nullptr;
    type_ = src.type_;
    num_ = src.num_;
    noperands_ = src.noperands_;
    AllocateOperands(MAX_OPS);
    for (size_t i = 0; i < src.noperands_; i++) {
        LOG(DEBUG, "copying operands_[" << i << "]...");
        operands_[i] = src.operands_[i];
    }
    return *this;
}

AFunction::AFunction(AFunction &&src) {
    LOG(DEBUG, "MOVE CTOR");
    ASSERT(!src.IsOperandOf(*this));
    delete [] operands_;
    operands_ = src.operands_;
    noperands_ = src.noperands_;
    type_ = src.type_;
    num_ = src.num_;
    src.operands_ = nullptr;
    src.type_ = AFunction::Element::INVTOK;
    return;
}

AFunction& AFunction::operator=(AFunction &&src) {
    LOG(DEBUG, "MOVE ASSIGN");
    ASSERT(!src.IsOperandOf(*this));
    delete [] operands_;
    operands_ = src.operands_;
    noperands_ = src.noperands_;
    type_ = src.type_;
    num_ = src.num_;
    src.operands_ = nullptr;
    src.type_ = AFunction::Element::INVTOK;
    return *this;
}

field_t AFunction::Eval(field_t x, field_t param /* = 0 */) const {
    switch(type_) {
        case AFunction::Element::SUM : {
            field_t val = 0;
            ASSERT(operands_ != nullptr);
            for (size_t i = 0; i < noperands_; i++) {
                val += operands_[i].Eval(x, param);
            }
            LOG(EVAL, "SUM " << val);
            return val;
        }

        case AFunction::Element::MUL : {
            field_t val = num_;
            ASSERT(operands_ != nullptr);
            for (size_t i = 0; i < noperands_; i++) {
                val *= operands_[i].Eval(x, param);
            }
            LOG(EVAL, "MUL " << val);
            return val;
        }

        case AFunction::Element::POW : {
            ASSERT(operands_ != nullptr);
            ASSERT(noperands_ == 1);
            field_t argval = operands_->Eval(x, param);
            field_t val = std::pow(argval, num_);
            LOG(EVAL, "POW " << val);
            return val;
        }

        case AFunction::Element::VAR :
            LOG(EVAL, "X " << x);
            return x;

        case AFunction::Element::PARAM :
            LOG(EVAL, "PARAM " << param);
            return param;

        case AFunction::Element::CONST :
            LOG(EVAL, "CONST " << num_);
            return num_;

        #define def(name, impl, token, derivative)      \
            case AFunction::Element::name : {           \
                ASSERT(operands_ != nullptr);           \
                ASSERT(noperands_ == 1);                \
                field_t argval = operands_->Eval(x);    \
                field_t val = impl(argval);             \
                LOG(EVAL, #name << val);                \
                return val;                             \
            }
        #include "elem_func_singleops-include.h"
        #undef def

        default : 
            UNREACHABLE();
    }
    UNREACHABLE();
}

field_t AFunction::EvalNumDerivative(field_t x, field_t step, size_t method_num) const {
    using namespace std::placeholders;
    auto f = std::bind(&AFunction::Eval, this, _1, 0);
    field_t h = step;
    switch (method_num) {
        case AFunction::FIRST :
            return (f(x + h) - f(x)) / h;

        case AFunction::SECOND :
            return (f(x) - f(x - h)) / h;

        case AFunction::THIRD :
            return (f(x + h) - f(x - h)) / (2 * h);

        case AFunction::FOURTH :
            return  4./3. * (f(x + h) - f(x - h)) / (2 * h) -
                    1./3. * (f(x + 2*h) - f(x - 2*h)) / (4 * h);

        case AFunction::FIFTH :
            return  3./2. * (f(x + h) - f(x - h)) / (2 * h) -
                    3./5. * (f(x + 2*h) - f(x - 2*h)) / (4 * h) +
                    1./10. * (f(x + 3*h) - f(x - 3*h)) / (6 * h);

        default :
            return 0;
    }
}

bool AFunction::IsOperandOf(const AFunction &other) const {
    if (&other == this) {
        return true;
    }

    if (other.operands_ != nullptr) {
        for (size_t i = 0; i < other.noperands_; i++) {
            if (IsOperandOf(other.operands_[i])) {
                return true;
            }
        }
    }
    return false;
}


bool AFunction::Optimize() {
    bool optimizable = true;
    
    return false;
    // WIP
    field_t val = 0;
    if (type_ == AFunction::Element::MUL) {
        val = num_;
    }

    for (size_t i = 0; i < noperands_; i++) {
        operands_[i].Optimize();
        switch (type_) {
            case AFunction::Element::SUM :
                if (!operands_[i].IsConst()) {
                    optimizable = false;
                } else {
                    val += operands_[i].num_;
                }
                break;

            case AFunction::Element::MUL :
                if (operands_[i].IsConst()) {
                    val *= operands_[i].num_;
                    if (i != noperands_ - 1) {
                        operands_[i] = std::move(operands_[noperands_ - 1]);
                        noperands_--;
                        i--;
                    }
                } else {
                    optimizable = false;
                }
                break;

            default :
                optimizable = false;
        }
    }

    if (optimizable) {
        AFunction temp{};
        temp.SetType(AFunction::Element::CONST);
        temp.num_ = val;
        *this = std::move(temp);
    } else {
        switch (type_) {
            case AFunction::Element::SUM :
                if (noperands_ == 1) {
                    *this = std::move(operands_[0]);
                    return true;
                }
                break;
            case AFunction::Element::MUL :
                if (noperands_ == 1 && num_ == 1) {
                    *this = std::move(operands_[0]);
                    return true;
                }
                break;

            case AFunction::Element::POW :
                if (operands_->IsConst())
                {
                    val = std::pow(operands_->num_, num_);

                }
            default :
                optimizable = false;
        }
    }

    return optimizable;
}

void AFunction::Dump(std::ostream *out)
{
    switch (type_) {
        case Element::VAR:
            (*out) << "x";
            break;
        case Element::PARAM:
            (*out) << "a";
            break;
        case Element::CONST:
            (*out) << num_;
            break;
        case Element::SUM:
            (*out) << '(';
            if (num_ != 0) {
                (*out) << num_ << " + ";
            }
            DumpOperands(out, '+');
            (*out) << ')';
            break;
        case Element::MUL:
            (*out) << '(';
            if (num_ == -1) {
                (*out) << '-';
            } else if (num_ != 1) {
                (*out) << num_;
            }
            DumpOperands(out, '*');
            (*out) << ')';
            break;
        case Element::INVTOK:
            (*out) << "invtok";
            break;
        default:
            UNREACHABLE();
    }
}

void AFunction::DumpOperands(std::ostream *out, char binop)
{
    ASSERT(noperands_ > 0);
    ASSERT(operands_ != nullptr);
    operands_[0].Dump(out);
    for (size_t i = 1; i < noperands_; i++) {
        (*out) << ' ' << binop << ' ';
        operands_[i].Dump(out);
    }
}

std::ostream& operator<<(std::ostream& ou, AFunction& func) {
    func.Dump(&ou);
/**
 * Debug dump:
 *
    static std::string prefix = "";
    prefix += '\t';
    prefix += '|';
    ou << prefix << "--------------------\n";
    ou << prefix << "type_:      "<< func.type_ << '\n';
    ou << prefix << "num_:      "<< func.num_<< '\n';
    ou << prefix << "noperands_: "<< func.noperands_ << '\n';
    ou << prefix << "operands_(" << func.operands_ << "): " << '\n';
    for (size_t i = 0; i < func.noperands_; i++) {
        ou << func.operands_[i];
    }
    ou << prefix << "--------------------\n";
    if (prefix.size() > 0) {
        prefix.erase(prefix.size() - 1);
        prefix.erase(prefix.size() - 1);
    }
 */
    return ou;
}

std::ostream& operator<< ( std::ostream& ou, AFunction::Element type)
{
    switch(type)
    {
        #define def(name, impl, token, derivative)      \
            case AFunction::Element::name : return ou << #name;
        #include "elem_func_def-include.h"
        #undef def
    }
    UNREACHABLE();
}

} // namespace cmath