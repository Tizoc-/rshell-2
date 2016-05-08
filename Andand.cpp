#include <iostream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

#include "Base.h"
#include "Andand.h"
#include "Command.h"

Andand::Andand()
{
    first = 0;
    second = 0;
}

Andand::Andand(Base* first, Base* second)
{
    this->first = first;
    this->second = second;
}

Andand::Andand(Base* first, Command* second)
{
    this->first = first;
    this->second = second;
}

int Andand::execute()
{
    int currStatus = 0;
    currStatus = first->execute();
    if(currStatus != 1)
    {
        return -1;
    }
    else
    {
        currStatus = second->execute();
        if(currStatus != 1)
        {
            return -1;
        }
    }
    return currStatus;
}