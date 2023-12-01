#ifndef __DEVS_STEPPER__
#define __DEVS_STEPPER__


#include <Arduino.h>

#include "logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;


struct Galvo {
    private: i32 cur = 0;
    private: f32 spd = 0;
    private: u8 dac, ref;

    private: struct $Galvo {
        u8 dac;
        u8 ref;
    };

    public: Galvo($Galvo args) {
        auto [dac, ref] = args;

        this->dac = dac;
        this->ref = ref;

        pinMode(dac, OUTPUT);
        pinMode(ref, OUTPUT);
    };

    public: func speed(f32 spd) -> None {
        this->spd = spd;
    };

    public: func target(i32 pos) -> u32 {
        if (pos >= 0) {
            dacWrite(dac, pos);
            digitalWrite(ref, LOW);
        } else {
            dacWrite(dac, 255 - abs(pos));
            digitalWrite(ref, HIGH);
        }
        u32 d = abs(pos - cur);
        u32 t = d * spd;
        this->cur = pos;
        return t;
    };
};


#endif
