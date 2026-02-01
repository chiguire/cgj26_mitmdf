#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include "game.h"

template<typename T>
T randomEnum() {
    return static_cast<T>(std::rand() % (static_cast<int>(T::___last)));
}

const char* stringVectorRand(const std::vector<std::string>& vec) {
    if (vec.empty()) {
        return nullptr;
    }
    return vec[mitmdf::g_programState->GetRandomInt() % vec.size()].c_str();
}
#endif