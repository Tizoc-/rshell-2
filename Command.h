#ifndef COMMAND_H
#define COMMAND_H
#include "Base.h"
class Command: public Base {
  private:
    string command;
    string argument;
  public:
    Command();
    Commands(string comm, string arg);
    void execute();
    string setCommand();
    string setArg();
};