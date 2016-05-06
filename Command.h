#ifndef COMMAND_H
#define COMMAND_H
#include "Base.h"
class Command: public Base {
  private:
    int runStat;
    vector<string> cmdVec;
  public:
    Command();
    int execute();
    void setCommand();
    void setArg();
    int getrunstat();
};
#endif