#ifndef __LASER_MASTER__
#define __LASER_MASTER__


#include <Arduino.h>
#include "../logs/log.cpp"
#include "../devs/@package.cpp"

#include "./Object.cpp"
#include "./Group.cpp"

#include "../types/@package.cpp"
using namespace types;



class Laser {
    private: struct $Laser {
        Engine *eng;
    };

    public: Laser($Laser args) {
        this->eng = args.eng;
    };

    public: func add(IList<Object*> items) {
        for (Object *item : items) {
            data.add(item);
        }
    };

    public: func tick() {
        auto [done, point] = data.tick();
        auto [x, y, c] = point;

        if (done != -1) {
            ledcWrite(0, c.r >> 3);
            ledcWrite(1, c.g >> 3);
            ledcWrite(2, c.b >> 3);
            delayMicroseconds(eng->target(x, y));
        }
    };

    private: Group data;
    private: Engine *eng;
};


#endif
