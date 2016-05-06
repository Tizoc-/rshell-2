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
#include "Command.h"
using namespace std;

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
    return currStatus
}