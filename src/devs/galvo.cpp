#ifndef __DEVS_STEPPER__
#define __DEVS_STEPPER__


#include <Arduino.h>

#include "logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;


struct Galvo {

    private: i32 current_position = 0;
    private: f32 max_speed = 0;
    private: u8 dac_pin, ref_pin;

    private: u32 operation_time = 0;

    private: struct $Galvo {
        u8 dac_pin;
        u8 ref_pin;
    };

    public: Galvo($Galvo args) {
        auto [dac_pin, ref_pin] = args;

        this->dac_pin = args.dac_pin;
        this->ref_pin = args.ref_pin;

        pinMode(dac_pin, OUTPUT);
        pinMode(ref_pin, OUTPUT);
    };

    public: func speed(f32 spd) -> None {  // Установка макс. скорости в 
        this->max_speed = spd;
    };

    public: func move(i32 pos) -> u32 {
        pos = constrain(pos, -255, 255);
        if (pos >= 0) {
            dacWrite(dac_pin, pos);
            digitalWrite(ref_pin, 0);
        }
        else {
            dacWrite(dac_pin, 255 - abs(pos));
            digitalWrite(ref_pin, 1);
        }
        operation_time = abs(pos - current_position) * max_speed;
        current_position = pos; 
        return operation_time;
    };
};


#endif
