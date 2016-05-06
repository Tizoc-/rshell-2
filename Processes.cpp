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
//Processes parses the code to split it into separate command lines
//should there be any semicolons present.
void Processes::parse(string input)
{
    vector<string> currCs
    istringstream inSS(input);
    string currString;
    //Main loop for parsing input that contains semicolons
    while(inSS >> currString);
    {
        bool hashtag = false;
        bool semicolon = false;
        currCs.size() = 0;
        currCs.push_back(currString);
        while(!semicolon)
        {
            inSS >> currString;
            //Tests for hashtag/semicolon presence
            if(currString.find("#") != string::npos)
            {
                hashtag = true;
            }
            if(currString.find(";") != string::npos)
            {
                semicolon = true;
            }
            if(!hashtag)
            {
                currCs.push_back(currString);
            }
        }
    }
    
    
}