#ifndef COMMAND_H
#define COMMAND_H
#include "Base.h"
class Command: public Base {
  private:
    vector<string> cmdVec;
  public:
    Command();
    int execute();
    void setCommand();
    void setArg();
};
#endif