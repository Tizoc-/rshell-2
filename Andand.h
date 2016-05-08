#ifndef ANDAND_H
#define ANDAND_H
#include "Base.h"
#include "Command.h"
class Andand: public Base
{
    public:
        Andand();
        Andand(Base* first, Command* second);
        Andand(Base* first, Base* second);
        int execute();
};
#endif