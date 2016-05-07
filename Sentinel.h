#ifndef SENTINEL_H
#define SENTINEL_H
#include "Base.h"
#include "Command.h"
class Sentinel: public Base {
    protected:
        Base* first;
        Base* second;
    public:
        Sentinel();
        Sentinel(Base* first, Command* second);
        Sentinel(Base* first, Base* second);
        ~Sentinel();
        virtual int execute();
};
#endif