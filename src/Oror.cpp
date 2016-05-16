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
#include "Oror.h"
#include "Command.h"
//Constructors are used to either initialize the pointers to 0,
//or initialize them to specific objects;
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
//This execute function is a lot more lenient with execution.
//By or logic, its second variable only executes if the first has failed.
int Oror::execute()
{
    int currStatus = 1;
    currStatus = first->execute();
    if(currStatus != 1)
    {
        currStatus = second->execute();
        return currStatus;
    }
    return currStatus;
}