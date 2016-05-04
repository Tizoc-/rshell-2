#ifndef SENTINEL_H
#define SENTINEL_H
#include "Base.h"
class Sentinel: public Base {
    protected:
        Base* first;
        Base* second;
    public:
        Sentinel();
        Sentinel(Base* first, Base* second);
        ~Sentinel();
        virtual void execute();
}
#endif