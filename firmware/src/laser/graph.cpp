#ifndef __LASER_GRAPH__
#define __LASER_GRAPH__


#include "logs/log.cpp"
#include "./Object.cpp"

#include "../types/@package.cpp"
using namespace std;


class Graph: public Object {

    private: struct $Graph {
        Func<f32(f32)> fn;
        f32 b, e, s;
    };

    public: Graph($Graph args) {
        auto [fn, b, e, s] = args;
        this->fn = fn;
        this->b = b;
        this->e = e;
        this->s = s;
    }

    public: func tick() -> State {
        x = x + s;
        bool done = x >= e;
        
        if (x >= e) {
            x = b;
        }

        i32 y = fn(x);
        return State {
            done,
            {
                x, y,
                W,
            }
        };
    }

    public: Func<f32(f32)> fn;
    public: f32 b, e, s;
    private: i32 x = 0;
};


#endif
