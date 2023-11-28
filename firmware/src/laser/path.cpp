#ifndef __LASER_STREAM__
#define __LASER_STREAM__


#include "../logs/log.cpp"
#include "./Object.cpp"

#include "../types/@package.cpp"
using namespace types;


class Path: public Object {
    public: func tick() -> State {
        if (!points.empty()) {
            Vec<i32> point = points[ind++];
            bool done = ind == points.size();
            ind = ind % points.size();

            return State {
                done,
                point[0],
                point[1],
                { u8(point[2] * 32), 0, 0}
            };
        } else {
            return State {-1, 0, 0, {0, 0, 0}};
        }
    }

    public: Vec<Vec<i32>> points;
    private: u32 ind = 0;
};


#endif
