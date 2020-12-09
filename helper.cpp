#ifndef HELPER
#define HELPER

#include <sys/stat.h>
#include <iostream>

inline bool exists (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

#endif
