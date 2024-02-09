#include "deps.cpp"

#include "types/@package.cpp"
using namespace types;

#include "devs/@package.cpp"
using namespace devs;

#include "sevs/@package.cpp"
using namespace sevs;


// Engines ports
#define ELD 6  // Engine L dir
#define ELS 7  // Engine L spd

#define ERD 5  // Engine R dir
#define ERS 4  // Engine L spd

// Sensors ports
#define SL A5  // Sensor L
#define SR A0  // Sensor R


// Iters/sec
#define IPS 50


Engine enl(ELS, ELD), enr(ERS, ERD);
Sensor sl(SL), sr(SR);

Line line(sl, sr);


func setup() -> None {
    Serial.begin(115200);
}


func loop() -> None {
    auto [lspd, rspd] = line.move(400);
    enl.spin(lspd);
    enr.spin(rspd);

    delay(1000 / IPS);
}
