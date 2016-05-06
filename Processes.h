#ifndef PROCESSES_H
#define PROCESSES_H
#include "Base.h"
class Processes: public Base {
    private:
        vector<Base*> currCmds
        string andand;
        string oror;
    public:
        Processes();
        ~Processes();
        void addCmd(Base* nxtCmd);
        void parse(string parse);
        void execute();
};
#endif