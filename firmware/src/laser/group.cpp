#ifndef __LASER_GROUP__
#define __LASER_GROUP__


#include "./Object.cpp"

#include "../types/@package.cpp"
using namespace types;



class Group: public Object {
    public: Group(): items({}) {}
    public: Group(Vec<Object*> items): items(items) {}

    public: func add(Object* obj) -> Group* {
        items.push_back(obj);
        return this;
    }

    public: func tick() -> State {
        State state = items[ind]->point();
        ind = (ind + abs(state.status)) % items.size();
        return state;
    }

    private: Vec<Object*> items;
    private: u32 ind = 0;
};


#endif
