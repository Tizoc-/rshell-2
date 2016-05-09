#ifndef PROCESSES_H
#define PROCESSES_H
#include "Base.h"
#include "Command.h"
#include "Andand.h"
#include "Oror.h"
class Processes {
    private:
        std::vector<Base*> currCmds;
    public:
        Processes();
        ~Processes();
        void parse(std::string parse);
        int execute();
        void reset();
};
#endif