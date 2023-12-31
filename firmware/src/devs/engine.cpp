#ifndef __DEVS_ENGINE__
#define __DEVS_ENGINE__


#include "galvo.cpp"

#include "../logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;


struct Engine {

    private: struct $Engine {
        Galvo *x, *y;
    };

    public: Engine($Engine args) {
        this->x = args.x;
        this->y = args.y;
    }

    public: func speed(f32 spd) -> None {
        this->x->speed(spd);
        this->y->speed(spd);
    };

    private: struct $pos {
        i32 x, y;
    };

    public: func target($pos pos) -> u32 {
        return target(pos.x, pos.y);
    };

    public: func target(i32 x, i32 y) -> u32 {
        u32 tx = this->x->target(x);
        u32 ty = this->y->target(y);
        return max(tx, ty);
    };

    private: Galvo *x, *y;
};


#endif
