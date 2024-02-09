#pragma once


#include <Arduino.h>

#include "../types/@package.cpp"
using namespace types;


struct Engine {
    Engine(u8 spd, u8 dir): pins({ spd, dir }) {
        pinMode(pins.spd, OUTPUT);
        pinMode(pins.dir, OUTPUT);
    }

    public: func spin(i32 spd) -> None {
        digitalWrite(pins.dir, spd > 0);
        analogWrite(pins.spd, abs(spd));
    }

    private: struct { u8 spd, dir; } pins;
};
