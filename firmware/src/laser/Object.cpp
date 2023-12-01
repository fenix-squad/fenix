#ifndef __LASER_OBJECT__
#define __LASER_OBJECT__


#include <Arduino.h>

#include "../math.cpp"
#include "../logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;

#include "./point.cpp"


struct State {
    i32 status;
    Point point;
};


class Object {
    public: virtual func tick() -> State = 0;


    public: virtual func display(bool flag) -> Object* {
        this->flag = flag;
        return this;
    }

    public: virtual func move(i32 x, i32 y) -> Object* {
        this->dx = x;
        this->dy = y;
        return this;
    }

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
        if (!flag) {
            stream.println("SKIP");
            return { -1, {0, 0, B} };
        }

        auto [status, point] = this->tick();
        auto [x, y, c] = point;

        x = x * sx;
        y = y * sy;

        f32 cs = math::cos(angle);
        f32 sn = math::sin(angle);

        return State {
            .status=status,
            .point={
                i32((x - rx) * cs - (y - ry) * sn + rx) + dx,
                i32((y - ry) * cs + (x - rx) * sn + ry) + dy,
                c,
            }
        };
    }

    private: f32 angle = 0;
    private: i32 dx = 0, dy = 0;
    private: i32 rx = 0, ry = 0;
    private: f32 sx = 1, sy = 1;
    private: bool flag = true;
};


#endif
