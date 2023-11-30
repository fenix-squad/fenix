#ifndef __LASER_STREAM__
#define __LASER_STREAM__


#include "../logs/log.cpp"
#include "./Object.cpp"

#include "../types/@package.cpp"
using namespace types;


class Path: public Object {
    public: func tick() -> State {
        if (!points.empty()) {
            Point point = points[ind++];
            bool status = (ind == points.size());
            ind = ind % points.size();
            return State { status, point };
        } else {
            return State {-1, {
                0, 0, B
            }};
        }
    }

    public: Vec<Point> points;
    private: u32 ind = 0;
};


#endif
