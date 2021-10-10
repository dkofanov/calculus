#include <iostream>
#define NDEBUG
enum LogType {
    ERROR,
    INFO,
    DEBUG,
    EVAL
};

#ifndef NDEBUG
#define LOG(type, message) \
	std::cerr << #type << " : " << message << std::endl;
#else
#define LOG(type, message) {                \
    if (type == ERROR)                      \
        std::cerr << "ERROR : " << message << std::endl;    \
}
#endif
