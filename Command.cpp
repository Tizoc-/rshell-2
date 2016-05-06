#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include "Command.h"
using namespace std;

Command::Command()
{
    runStat = 0;
    cmdVec.size() = 0;
}

void Command::addPart(string comm)
{
    cmdVec.push_back(comm);
}

int Command::execute()
{
    int status = 0;
    char *parameter;
    parameter = new char[cmdVec.size()];
    for(int i = 0; i < cmdVec.size(); ++i)
    {
        parameter[i] = cmdVec[i];
    }
    int pid = -1;
    
    pid = fork();
    
    if(pid == 0)
    {
        status = execvp(cmdVec[0], cmdVec);
    }
    else
    {
        wait(0);
        return status;
    }
}

int Command::getrunstat()
{
    return runStat;
}