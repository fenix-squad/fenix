#ifndef __LASER_EQUATION__
#define __LASER_EQUATION__


#include "logs/log.cpp"
#include "./Object.cpp"

#include "../types/@package.cpp"
using namespace std;


class Equation: public Object {
    public: Equation(Func<bool(f32, f32)> fn, i32 xs, i32 ys, i32 xf, i32 yf):
        fn(fn), xs(xs), ys(ys), xf(xf), yf(yf), x(xs), y(ys) {}

    public: func tick() -> State {
        x = x + 1;
        if (x >= xf) {
            x = xs;
            y = y + 1;
        }

        bool done = y >= yf;

        if (fn(x, y)) {
            return State {
                done,
                x, y,
                {32, 0, 0},
            };
        } else {
            return State {done ? 1 : -1, 0, 0, {0, 0, 0}};
        }
    }

    public: Func<bool(f32, f32)> fn;
    public: i32 xs, ys, xf, yf;
    private: i32 x = 0, y = 0;
};


#endif
