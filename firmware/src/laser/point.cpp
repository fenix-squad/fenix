#ifndef __LASER_POINT__
#define __LASER_POINT__


#define WHITE rgb(255, 255, 255)
#define BLACK rgb(  0,   0,   0)

#define W WHITE
#define B BLACK

#include "../types/@package.cpp"
using namespace types;


struct Color {
    u8 r, g, b;
};

inline func rgb(u8 r, u8 g, u8 b) -> Color {
    return { r, g, b };
}

inline func rgb(u8 s) -> Color {
    return { s, s, s };
}


struct Point {
    i32 x, y;
    Color c;
};


#endif
