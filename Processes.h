#ifndef PROCESSES_H
#define PROCESSES_H
#include "Base.h"
class Processes: public Base {
    private:
        std::vector<Base*> currCmds;
        std::string andand;
        std::string oror;
    public:
        Processes();
        ~Processes();
        void addCmd(Base* nxtCmd);
        void parse(std::string parse);
        int execute();
};
#endif