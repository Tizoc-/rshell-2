#ifndef PROCESSES_H
#define PROCESSES_H
#include "Base.h"
#include "Command.h"
#include "Andand.h"
#include "Oror.h"
class Processes {
    //Declares the vector of Base pointers that the object
    //will use for executables.
    private:
        std::vector<Base*> currCmds;
    //Standard declaration of constructors, destructor, and
    //subsequent functionality.
    public:
        Processes();
        ~Processes();
        void parse(std::string parse);
        int execute();
        void reset();
};
#endif