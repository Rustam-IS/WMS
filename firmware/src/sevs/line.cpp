#pragma once


#include "../devs/@package.cpp"
using namespace devs;

#include "../types/@package.cpp"
using namespace types;

#include "../tools.cpp"
using namespace tools;


struct Vals {
    i32 ls, rs;
    bool done;
};


namespace Line {

    struct Computer {
        Computer(Sensor sl, Sensor sr): sl(sl), sr(sr) {}

        public: func move(i32 spd = 120, float k = 0.32) -> Vals {
            i32 diff = (sr.get() - sl.get()) * k;
            return Vals { spd + diff, spd - diff, false };
        }

        public: func time(u32 time, i32 spd = 120, float k = 0.32) -> Vals {
            if (clock == 0) {
                clock = millis() + time;
            }

            if (millis() < clock) {
                return move(spd, k);
            } else {
                clock = 0;
                return Vals { 0, 0, true };
            }
        }

        public: func cross(i32 spd = 120, float k = 0.32, i32 b = 600) -> Vals {
            if (sl.get() < b || sr.get() < b) {
                return move(spd, k);
            } else {
                return Vals { 0, 0, true };
            }
        }

        public: func turn(i32 spd = 160, i32 b = 600) -> Vals {
            if (!(lf && rf)) {
                var lv = spd < 0 ? sl.get() : sr.get();
                var rv = spd < 0 ? sr.get() : sl.get();

                lf = lf || lv > b;
                rf = rf || (lf && rv > b);

                return { -spd, spd, false };
            } else {
                lf = false; rf = false;
                return { 0, 0, true, };
            }
        }

        public: func turnL(i32 spd = 160, i32 b = 600) -> Vals {
            return turn(spd, b);
        }

        public: func turnR(i32 spd = 160, i32 b = 600) -> Vals {
            return turn(-spd, b);
        }

        private: Sensor sl, sr;
        private: bool lf = false,
                      rf = false;
        private: u32 clock = 0;
    };

    struct Executor {
        public: Executor(Engine el, Engine er): el(el), er(er) {}

        public: func spin(i32 ls, i32 rs) -> None {
            el.spin(ls);
            er.spin(rs);
            delay(5);
        }

        private: Engine el, er;
    };

    struct Line {
        Line(Computer c, Executor e): c(c), e(e) {}

        public: func time(u32 time, i32 spd = 120, float k = 0.32) -> None {
            while (!(state = c.time(time, spd, k)).done) {
                e.spin(state.ls, state.rs);
            } e.spin(0, 0);
        }

        public: func cross(i32 spd = 120, float k = 0.32, i32 b = 600) -> None {
            while (!(state = c.cross(spd, k, b)).done) {
                e.spin(state.ls, state.rs);
            } e.spin(0, 0);
        }

        public: func turn(i32 spd = 160, i32 b = 600) -> None {
            while (!(state = c.turn(spd, b)).done) {
                e.spin(state.ls, state.rs);
            } e.spin(0, 0);
        }

        public: func turnL(i32 spd = 160, i32 b = 600) -> None {
            while (!(state = c.turnL(spd, b)).done) {
                e.spin(state.ls, state.rs);
            } e.spin(0, 0);
        }

        public: func turnR(i32 spd = 160, i32 b = 600) -> None {
            while (!(state = c.turnR(spd, b)).done) {
                e.spin(state.ls, state.rs);
            } e.spin(0, 0);
        }

        private: Computer c;
        private: Executor e;
        private: Vals state;
    };
};
