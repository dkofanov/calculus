#ifndef LIB_LOG_H_INCLUDED
#define LIB_LOG_H_INCLUDED

#include <iostream>
#define NDEBUG
#define NDEBUG_MATRIX
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

#ifndef NDEBUG_MATRIX
#define LOG_MATRIX(type, message) \
	std::cerr << #type << " : " << message << std::endl;
#else
#define LOG_MATRIX(type, message) {                \
    if (type == ERROR)                      \
        std::cerr << "ERROR : " << message << std::endl;    \
    if (type == INFO)                      \
        std::cout << "INFO : " << message << std::endl;    \
}
#endif

#endif  // LIB_LOG_H_INCLUDED
