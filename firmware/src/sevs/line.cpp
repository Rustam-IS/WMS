#pragma once


#include "../devs/@package.cpp"
using namespace devs;

#include "../types/@package.cpp"
using namespace types;


struct Vals {
    i32 ls, rs;
};


struct Line {
    Line(Sensor sl, Sensor sr): sl(sl), sr(sr) {}

    public: func move(i32 spd, float k = 3) -> Vals {
        i16 diff = (sl.get() - sr.get()) * k;
        return { spd + diff, spd - diff };
    }

    public: func cross(i32 spd) -> None {
        
    }

    private: Sensor sl, sr;
};
