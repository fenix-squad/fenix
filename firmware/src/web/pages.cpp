#ifndef __WEB_INDEX__
#define __WEB_INDEX__

#include "SPIFFS.h"

#include "../types/@package.cpp"
using namespace types;

#include "types.cpp"


inline u8 spd = 4;

namespace Web {
    inline func index(Request *request) -> None {
        request->send(SPIFFS, "/index.html", "text/html");
    }

    inline func config(Request *request) -> None {
        u8 count = request->params();

        for (u8 ind = 0; ind < count; ind++) {
            Parameter *param = request->getParam(ind);
            if (param->name() == "spd") {
                spd = param->value().toInt();
            }

            request->send(200, "text/html", "");
        }
    }
};


#endif
