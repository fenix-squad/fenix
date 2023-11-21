#ifndef __DEVS_ENGINE__
#define __DEVS_ENGINE__


#include "galvo.cpp"

#include "../logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;


struct Couple {

    private: struct $Couple {
        Galvo *x, *y;
    };

    public: Couple($Couple args) {
        this->x = args.x;
        this->y = args.y;
    }

    public: func speed(f32 spd) -> None {
        this->x->speed(spd);
        this->y->speed(spd);
    };

    private: struct $target {
        i32 x, y;
    };

    public: func move($target args) -> u32 {
        u32 time_x = this->x->move(args.x);
        u32 time_y = this->y->move(args.y);
        return max(time_x, time_y);
    };

    private: Galvo *x, *y;
};


#endif
