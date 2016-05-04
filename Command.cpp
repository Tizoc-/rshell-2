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
    cmdVec.size() = 0;
}

void Command::addPart(string comm)
{
    cmdVec.push_back(comm);
}

int Command::execute()
{
    int pid = -1;
    
    pid = fork();
}