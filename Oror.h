#ifndef OROR_H
#define OROR_H
#include "Base.h"
#include "Command.h"
#include "Sentinel.h"
class Oror: public Sentinel
{
    public:
        Oror();
        Oror(Base* first, Command* second);
        Oror(Base* first, Base* second);
        int execute();
};
#endif