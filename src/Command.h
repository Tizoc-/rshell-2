#ifndef COMMAND_H
#define COMMAND_H
#include "Base.h"
class Command: public Base {
  //Unlike its parent class, this class has variables to
  //check its execution and the parameters of said execution.
  protected:
    int runStat;
    std::vector<std::string> cmdVec;
  public:
  //Included here are standard constructors and functionality.
    Command();
    Command(std::vector<std::string> currCommand);
    int execute();
    int getrunstat();
};
#endif
