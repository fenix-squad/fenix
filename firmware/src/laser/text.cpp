#ifndef __LASER_TEXT__
#define __LASER_TEXT__


#include <Arduino.h>
#include "../assets/fonts/default.cpp"

#include "./Object.cpp"
#include "../logs/log.cpp"

#include "../types/@package.cpp"
using namespace types;


class Text : public Object {
    public: Text(String text, Font *font = &Default) {
        this->text = text;
        this->font = font;
        this->size = text.length();
    };

    public: func tick() -> State {
        Font font = *(this->font);
        
        step = step + 1;
        if (step == font[text[ind]].size()) {
            ind = (ind + 1) % size;
            step = 0;
        }
        Vec<Point> ch = font[text[ind]];
        auto [x, y, c] = ch[step];

        x = x + abs(size / 2 - ind) * 15 * (ind < size / 2 ? -1 : 1);
        y = y;

        bool status = (ind == size - 1) and (step == ch.size() - 1);
        return State {
            .status=status,
            .point={ x, y, c }
        };
    };

    private: String text;
    private: Font *font;

    private: u16 size = 0,
                 ind = 0,
                 step = 0;
};


#endif
