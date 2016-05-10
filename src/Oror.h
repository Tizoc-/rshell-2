#ifndef OROR_H
#define OROR_H
#include "Base.h"
#include "Command.h"
class Oror: public Base
{
    //Similar to its sibling Andand class, this function declares
    //its pointers and basic functionality.
    private:
        Base * first;
        Base * second;
    public:
        Oror();
        Oror(Base* first, Command* second);
        Oror(Base* first, Base* second);
        int execute();
};
#endif