#ifndef SENTINEL_H
#define SENTINEL_H
#include "Base.h"
class Sentinel: public Base {
    protected:
        Base* first;
        Base* second;
    public:
        virtual void execute();
}