#ifndef __FONT__
#define __FONT__


#include "../../types/@package.cpp"
using namespace types;


struct Point {
    i32 x, y;
    bool s;
};

using Font = Map<char, Vec<Point>>;


#endif
