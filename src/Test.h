#ifndef TEST_H
#define TEST_H
#include "Command.h"
class Test : public Command {
    public:
        Test(std::vector<std::string> commandVec);
        int execute();
        int getrunstat();
};
#endif