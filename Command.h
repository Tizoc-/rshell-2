#ifndef COMMAND_H
#define COMMAND_H
#include "Base.h"
class Command: public Base {
  private:
    int runStat;
    std::vector<std::string> cmdVec;
  public:
    Command();
    Command(std::vector<std::string> currCommand);
    int execute();
    int getrunstat();
};
#endif