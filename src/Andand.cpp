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
//Standard constructor, sets pointers to 0
Andand::Andand()
{
    first = 0;
    second = 0;
}
//Sets pointers to passed in parameters
//Usually used for the first command in a semicolon-ended line
Andand::Andand(Base* first, Base* second)
{
    this->first = first;
    this->second = second;
}
//Used to connect connectors to ohters connector
//Becuase the logic builds off of itself until it hits semicolons,
//connectors need to point to other connectors to "connect" the logic
Andand::Andand(Base* first, Command* second)
{
    this->first = first;
    this->second = second;
}
//Executes the object's first and second pointers.
//If the first fails, it will not execute the second, and return -1,
//which indicates failer.
int Andand::execute()
{
    int currStatus = 0;
    currStatus = first->execute();
    if(currStatus != 0)
    {
        return -1;
    }
    else
    {
        currStatus = second->execute();
        if(currStatus != 0)
        {
            return -1;
        }
    }
    return currStatus;
}