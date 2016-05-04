#ifndef COMMAND_H
#define COMMAND_H
#include "Base.h"
class Command: public Base {
  private:
    string command;
    string argument;
  public:
    Command();
    Command(string comm, string arg);
    void execute();
    void setCommand();
    void setArg();
};
#endif