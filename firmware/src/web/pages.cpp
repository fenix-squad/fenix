#ifndef __WEB_INDEX__
#define __WEB_INDEX__


#include "SPIFFS.h"

#include "../logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;

#include "types.cpp"


inline u8 spd = 3;

namespace Web {
    inline func index(Request *request) -> None {
        request->send(SPIFFS, "/index.html", "text/html");
    }
};


#endif
