#ifndef PROCESSES_H
#define PROCESSES_H
#include "Base.h"
class Processes: public Base {
    private:
        vector<Base*> currCmds
        vector<string> connectors
    public:
        void parse();
        void execute();
};
#endif