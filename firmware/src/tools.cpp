#pragma once

#define DEBUG


#include <Arduino.h>

#include "./types/@package.cpp"
using namespace types;


namespace tools {
    template<typename T>
    func swap(T &a, T &b) -> None {
        T c(a);
        a = b;
        b = c;
    }

    template<typename T>
    func debug(T arg) -> None {
        #ifdef DEBUG
            Serial.println(arg);
        #endif
    }
}
