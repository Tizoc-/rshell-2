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
#include "Sentinel.h"
#include "Oror.h"
#include "Command.h"
using namespace std;

Oror::Oror()
{
    first = 0;
    second = 0;
}

Oror::Oror(Base* first, Base* second)
{
    this->first = first;
    this->second = second;
}

Oror::Oror(Base* first, Command* second)
{
    this->first = first;
    this->second = second;
}

int Oror::execute()
{
    int currStatus = 0;
    currStatus = first->execute();
    currStatus = second->execute();
    if(currStatus != 1)
    {
        return -1;
    }
    return currStatus;
}