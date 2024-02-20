#pragma once


#include <Arduino.h>

#include "../types/@package.cpp"
using namespace types;


struct Sensor {
    Sensor(u8 pin): pin(pin) {
        pinMode(pin, INPUT);
    }

    public: func get() -> i32 {
        return analogRead(pin);
    }

    private: u8 pin;
};
