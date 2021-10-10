#ifndef MACRO_H_INCLUDDED
#define MACRO_H_INCLUDDED

#include <cassert>

#define ASSERT(a) assert(a)

#define UNREACHABLE(a) { \
    LOG(ERROR, "UNREACHABLE CODE: " a);  \
    ASSERT(false);                      \
    __builtin_unreachable();\
}

#endif  // MACRO_H_INCLUDDED
