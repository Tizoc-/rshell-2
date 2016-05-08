#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <algorithm>
using namespace std;

#include "Base.h"
#include "Command.h"

Command::Command()
{
    runStat = 0;
    cmdVec.resize(0);
}

Command::Command(vector<string> currCommand)
{
    runStat = 0;
    cmdVec = currCommand;
}

char *convert(const std::string & s)
{
   char *pc = new char[s.size()+1];
   std::strcpy(pc, s.c_str());
   return pc; 
}

int Command::execute()
{
    runStat = 1;
    char** cstrings = new char*[cmdVec.size()];
    for(size_t i = 0; i < this->cmdVec.size(); ++i)
    {
        cstrings[i] = new char[cmdVec[i].size() + 1];
        std::strcpy(cstrings[i], cmdVec[i].c_str());
    }
    pid_t pid = fork();
    if(pid == 0)
    {
        //Child process!
        runStat = execvp(cstrings[0], cstrings);
        if(runStat < 0)
        {
            perror("Command has failed to execute");
            return runStat;
        }
    }
    int status;
    waitpid(pid, &status, 0);
    for(unsigned i = 0; i < cmdVec.size(); ++i)
    {
        delete[] cstrings[i];
    }
    delete[] cstrings;
    return runStat;
}

int Command::getrunstat()
{
    return runStat;
}