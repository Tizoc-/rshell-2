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
//Runstat will be used by the command object in order to relay
//its execution status; that is, if it failed or not.
//It also has a vector that contains the operands of its command.
Command::Command()
{
    runStat = 0;
    cmdVec.resize(0);
}
//This constructor is similar to default, except that it directly
//initializes the cmdVec variable.
Command::Command(vector<string> currCommand)
{
    runStat = 0;
    cmdVec = currCommand;
}
//This is used by execute to tranform a s vector of strings to
//a char * array for use in execvp.
char *convert(const std::string & s)
{
   char *pc = new char[s.size()+1];
   std::strcpy(pc, s.c_str());
   return pc; 
}
//The execute function starts by converting the string vector to a
//char * array. Then, we fork a process to run a command.
//Because execvp only returns if it fails to execute a command,
//runStat gets updated if and only if the command fails.
//Finally, it clens up the dynamic allocation of the transformation process,
//and returns the runStat.
int Command::execute()
{
    if(cmdVec.at(0) == "exit")
    {
        exit(0);
    }
    runStat = 1;
    char** cstrings = new char*[cmdVec.size() + 1];
    unsigned i = 0;
    for(; i < this->cmdVec.size(); ++i)
    {
        cstrings[i] = new char[cmdVec[i].size() + 1];
        std::strcpy(cstrings[i], cmdVec[i].c_str());
    }
    cstrings[i] = NULL;
    pid_t pid = fork();
    if(pid == 0)
    {
        //Child process!
        runStat = execvp(cstrings[0], cstrings);
        if(runStat != 0)
        {
            perror("Command has failed to execute");
            return -1;
        }
    }
    int status;
    waitpid(pid, &status, 0);
    for(unsigned i = 0; i < cmdVec.size(); ++i)
    {
        delete[] cstrings[i];
    }
    delete[] cstrings;
    return status;
}
//Simple accessor to access the runStat.
int Command::getrunstat()
{
    return runStat;
}