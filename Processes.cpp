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
#include "Command.h"
#include "Andand.h"
#include "Oror.h"
using namespace std;
//Simple constructor
Processes::Processes()
{
    currCmds.size() = 0;
    string andand == "&&";
    string oror = "||";
}
//Simple destructor
Processes::~Processes()
{
    for(int i = 0; i < currCmds; ++i)
    {
        delete currCmds.at(i);
    }
}
//This is used to add connector objects to the currCmds vector
void Processes::addCmd(Base* nxtCmd)
{
    this->currCmds.push_back(nxtCmd);
}
//This executes all objects within the object's currCmds vector
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
    while(input.find(";") == string::npos);
    {
        bool hashtag = false;
        bool semicolon = false;
        currCs.size() = 0;
        while(!semicolon)
        {
            inSS >> currString;
            input.erase(0, currString.size());
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
                if(!semicolon)
                {
                    currCs.push_back(currString);
                }
                else
                {
                    currString.erase(currString.size() - 1);
                    currCs.push_back(currString);
                }
            }
        }
        bool detected = false;
        for(unsigned j = 0; j < currCs.size(); ++j)
        {
            if(currCs.at(i) == andand || currCs.at(i) == oror)
            {
                detected = true;
                break;
            }
        }
        if(detected)
        {
            string prevConnector;
            string nextConnector;
            vector<string> firstCommand;
            unsigned i = 0;
            while(currCs.at(i) != andand && currCs.at(i) != oror)
            {
                firstCommand = currCs.at(i);
                ++i;
            }
            prevConnector = currCs.at(i);
            Command *temp3 = new Command(firstComand);
            cmdVec.push_back(temp3);
            ++i;
            bool dealing = false;
            vector<string> currCommand;
            for(i; i < currCs.size(); ++i)
            {
                currCommand.push_back(currCs.at(i));
                if(currCs.at(i) == andand || currCs.at(i) == oror)
                {
                    currCommand.pop_back();
                    Command *temp = new Command(currCommand);
                    nextConnector = currCs.at(i);
                    if(prevConnector == andand)
                    {
                        Andand *temp2 = new Andand(cmdVec.at(cmdVec.size() - 1), temp);
                        cmdVec.pop_back();
                        cmdVec.push_back(temp2);
                        currCommand.resize(0);
                    }
                    else
                    {
                        Oror * temp2 = new Oror(cmdVec.at(cmdVec.size() - 1), temp);
                        cmdVec.pop_back();
                        cmdVec.push_back(temp2);
                        currCommand.resize(0);
                    }
                }
            }
        }
        else {
            vector<string> currCommand;
            for(unsigned k = 0; k < currCs.size() ++i)
            {
                currCommand.push_back(currCs.at(i));
            }
            Command *temp = new Command(currCommand);
            cmdVec.push_back(temp);
        }
    }
    //Second loop that parses the remaining part of the input
    //Also, if the input never contained any semicolons,
    //the parse will go straight to this part of the code
    bool hashtag2 = false;
}