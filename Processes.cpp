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
    int temp = 0;
    for(int i = 0; i < currCmds.size(); ++i)
    {
        temp = currCmds.at(i)->execute();
    }
}

void Processes::parse(string input)
{
    vector<string> currCs
    istringstream inSS(input);
    string currString;
    while(inSS >> currString);
    {
        bool hashtag = false;
        while(currLine != ";")
        {
            currCs.size() = 0;
            if(string.find("#") != string::npos)
            {
                hashtag = true;
            }
            if(!hashtag)
            {
                currCs.push_back(currString)
            }
            inSS >> currString;
        }
        
    }
    
    
}