#ifndef ANDAND_H
#define ANDAND_H
#include "Base.h"
#include "Command.h"
#include "Sentinel.h"
class Andand: public Sentinel
{
    public:
        Andand();
        Andand(Base* first, Command* second);
        Andand(Base* first, Base* second);
        int execute();
};
#endif