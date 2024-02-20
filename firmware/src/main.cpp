/* Including libraries */
#include "deps.cpp"

#include "types/@package.cpp"
using namespace types;

#include "devs/@package.cpp"
using namespace devs;

#include "sevs/@package.cpp"
using namespace sevs;


/* Defining ports */

// Engines ports
#define ELD 7  // Engine L dir
#define ELS 6  // Engine L spd

#define ERD 4  // Engine R dir
#define ERS 5  // Engine L spd

// Grab ports
#define SG 3   // Servo
#define SA 8   // Stepper A
#define SB 9   // Stepper B
#define SC 10  // Stepper C
#define SD 11  // Stepper D

// Sensors ports
#define SL A5  // Sensor L
#define SR A4  // Sensor R

// Iters/sec
#define IPS 50


/* Connecting devices */
Engine enl(ELS, ELD), enr(ERS, ERD);
Sensor sl(SL), sr(SR);


// Stepper y(2038, SA, SB, SC, SD);
Servo g;  // 0 - 70


/* Creating services */
Line::Computer comp(sl, sr);
Line::Executor exec(enl, enr);
Line::Line line(comp, exec);


i32 items[2] = {4, 4};


func setup() -> None {
    Serial.begin(9600);

    g.attach(SG);
    g.write(100);

    pinMode(13, OUTPUT);
}


i32 dest = -1;
i32 item = -1;
Vals state;

i32 cnt = 0;


const u8 SIZE = 2;
u8 BUFFER[SIZE];

func spin(i32 s, i32 t) -> None {
    enl.spin(s);
    enr.spin(s);
    delay(t);
    enl.spin(0);
    enr.spin(0);
}


func spin(i32 sl, i32 sr, i32 t) -> None {
    enl.spin(sl);
    enr.spin(sr);
    delay(t);
    enl.spin(0);
    enr.spin(0);
}



func loop() -> None {
    if (Serial.available() >= SIZE) {
        digitalWrite(13, HIGH);
        Serial.readBytes(BUFFER, SIZE);
        dest = BUFFER[0];
        item = BUFFER[1];
    }

    Serial.print(sl.get());
    Serial.print(" ");
    Serial.println(sr.get());

    if (dest == 1) {
        debug("Dest = 1");

        debug("H -> P");
        cnt = (items[item] + 1) / 2 + 1;
        for (var ind = 0; ind < cnt; ind++) {
            line.cross();
            spin(160, 200);
        }
        line.time(300);
        delay(500);

        debug("R(90)");
        if (item == 0) {
            line.turnL();
        } else {
            line.turnR();
        }
        delay(500);

        debug("T -> P");
        line.cross();
        spin(160, 200);
        line.time(400);
        delay(500);

        debug("grab");
        g.write(140);
        delay(500);

        spin(-200, 400);

        debug("R(180)");
        line.turn();
        spin(-200, 200, 100);
        line.turn();
        delay(500);

        debug("P -> T");
        line.cross();
        spin(160, 200);
        line.time(700);
        delay(500);

        debug("R(90)");
        if (item == 0) {
            line.turnL();
        } else {
            line.turnR();
        }
        delay(500);

        debug("T -> X");
        cnt = 2 - (items[item] + 1) / 2;
        for (var ind = 0; ind < cnt; ind++) {
            line.cross();
            spin(160, 200);
        }
        delay(500);

        debug("X -> D");
        line.cross();
        spin(160, 300);
        delay(500);

        debug("Ungrab");
        g.write(100);
        delay(500);

        debug("R(180)");
        line.turn();
        spin(-200, 200, 200);
        line.turn();
        delay(500);
        
        debug("D -> H");
        cnt = 3;
        for (var ind = 0; ind < cnt; ind++) {
            line.cross();
            spin(160, 200);
        }
        spin(160, 200);
        line.time(1000);
        delay(500);

        spin(-200, 200, 200);
        line.turn();
        delay(500);
 
        dest = -1;
        item = -1;
    }

    delay(1000 / IPS);
}
