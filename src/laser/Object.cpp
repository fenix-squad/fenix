#ifndef __LASER_OBJECT__
#define __LASER_OBJECT__


#include <Arduino.h>
#include <cmath>

#include "../logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;


struct Color {
    u8 r, g, b;
};


struct State {
    i32 done;
    i32 x, y;
    Color c;
};


class Object {
    public: virtual func tick() -> State = 0;

    public: virtual func rotate(f32 angle, i32 x = 0, i32 y = 0) -> Object* {
        this->angle = angle;
        this->rx = x;
        this->ry = y;
        return this;
    }

    public: virtual func scale(f32 sx, f32 sy) -> Object* {
        this->sx = sx;
        this->sy = sy;
        return this;
    }

    public: virtual func scale(f32 s) -> Object* {
        this->sx = this->sy = s;
        return this;
    }

    public: func virtual point() -> State {
        auto [done, x, y, c] = this->tick();
        x = x * sx;
        y = y * sy;
        return State {
            done,
            i32((x - rx) * cos(angle) - (y - ry) * sin(angle) + rx),
            i32((y - ry) * cos(angle) + (x - rx) * sin(angle) + ry),
            c,
        };
    }

    private: f32 angle = 0;
    private: i32 rx = 0, ry = 0;
    private: f32 sx = 1, sy = 1;
};


#endif
