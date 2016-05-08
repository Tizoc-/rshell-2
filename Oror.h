#ifndef OROR_H
#define OROR_H
#include "Base.h"
#include "Command.h"
class Oror: public Base
{
    public:
        Oror();
        Oror(Base* first, Command* second);
        Oror(Base* first, Base* second);
        int execute();
};
#endif