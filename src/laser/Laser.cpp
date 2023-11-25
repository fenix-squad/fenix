#ifndef __LASER_MASTER__
#define __LASER_MASTER__


#include <Arduino.h>
#include "../logs/log.cpp"

#include "../devs/@package.cpp"
#include "./Object.cpp"

#include "../types/@package.cpp"
using namespace types;


class Laser {
    private: struct $Laser {
        Engine *eng;
        u8 laser;
    };

    public: Laser($Laser args) {
        this->eng = args.eng;
        this->laser = args.laser;

        pinMode(args.laser, OUTPUT);
    };

    public: func add(IList<Object*> items) {
        for (Object *item : items) {
            objs.push_back(item);
        }
    };

    public: func tick() {
        auto [done, x, y, c] = objs[ind]->point();

        if (done != -1) {
            ledcWrite(0, c.r);
            delayMicroseconds(eng->target({ .x=x, .y=y }));
        }

        ind = (ind + abs(done)) % objs.size();
    };

    public: Vec<Object*> objs;  // List  of objects to draw
    private: u32 ind = 0;        // Index of current object

    private: Engine *eng;  // Laser engine
    private: u8 laser;     // Laser PIN
};


#endif
