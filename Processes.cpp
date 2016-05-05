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
#include "Base.h"
using namespace std;

Processes::Processes()
{
    currCmds.size() = 0;
    string andand == "&&";
    string oror = "||";
    string semicolon = ";";
    string comment = "#";
}

Processes::~Processes()
{
    for(int i = 0; i < currCmds; ++i)
    {
        delete currCmds.at(i);
    }
}

void Processes::addCmd(Base* nxtCmd)
{
    this->currCmds.push_back(nxtCmd);
}

void Processes::execute()
{
    
}

void Processes::parse(string input)
{
<<<<<<< HEAD
    
    
=======
    vector<string> currCs
    istringstream inSS(input);
    string currString;
    while(inSS >> currString);
    {
        bool hashtag = false;
        while(currLine != ";")
        {
            
            if(!hashtag)
            {
                currCs.push_back(currString)
            }
            else {
                
            }
            inSS >> currString;
        }
        
    }
>>>>>>> 0b8750180df1294e6b0a863a99205522bcbd62cb
}