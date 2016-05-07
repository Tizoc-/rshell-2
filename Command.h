#ifndef COMMAND_H
#define COMMAND_H
#include "Base.h"
class Command: public Base {
  private:
    int runStat;
    std::vector<std::string> cmdVec;
  public:
    Command();
    int execute();
    void addPart(std::string comm);
    int getrunstat();
};
#endif