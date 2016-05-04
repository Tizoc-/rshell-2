#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include "Command.h"
using namespace std;

Command::Command()
{
    command = "";
    argument = "";
}

Command::Command(string comm, string arg)
{
    command = comm;
    argument = arg;
}

void Command::setCommand(string comm)
{
    this->command = comm;
}

void Command::setArg(string arg)
{
    this->argument = arg;
}

void Command::execute()
{
    
}