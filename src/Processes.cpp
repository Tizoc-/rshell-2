#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
using namespace std;

#include "Processes.h"
#include "Base.h"
#include "Command.h"
#include "Andand.h"
#include "Oror.h"
#include "Test.h"

//Simple constructor
Processes::Processes()
{
    currCmds.resize(0);
}
//Simple destructor
Processes::~Processes()
{
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        delete currCmds.at(i);
    }
}
//This executes all objects within the object's currCmds vector
int Processes::execute()
{
    int temp = 0;
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        temp = currCmds.at(i)->execute();
    }
    return temp;
}
//Processes parses the code to split it into separate command lines
//should there be any semicolons present.
void Processes::parse(string input)
{
    vector<string> currCs;
    istringstream inSS(input);
    string currString;
    //Main loop for parsing input that contains semicolons
    if(count(input.begin(), input.end(), ';') >= 1)
    {
        while(input.find(";") != string::npos)
        {
            if(count(input.begin(), input.end(), ';') == 1 
                && input.find(";") != input.size() - 1 
                && (input.at(input.find(";") + 1)) == '"')
            {
                break;
            }
            if(count(input.begin(), input.end(), ';') == 1 && input.find(";") == input.size() - 1)
            {
                break;
            }
            bool hashtag = false;
            bool semicolon = false;
            bool first = false;
            currCs.resize(0);
            while(!semicolon)
            {
                inSS >> currString;
                input.erase(0, currString.size() + 1);
                //Tests for hashtag/semicolon presence
                if(currString.find("#") != string::npos)
                {
                    hashtag = true;
                    if(currString.find("#") != 0 && (!first))
                    {
                        currCs.push_back(currString);
                        first = true;
                    }
                }
                if(currString.find(";") != string::npos)
                {
                    semicolon = true;
                }
                string temp = "";
                for(unsigned g = 0; g < currString.size(); ++g)
                {
                    if(currString.at(g) == '"')
                    {
                        if(g > 0 && currString.at(g - 1) == '\\')
                        {
                            temp += currString.at(g);
                        }
                    }
                    else if(currString.at(g) != '\\')
                    {
                        temp += currString.at(g);
                    }
                }
                currString = temp;
                if(!hashtag)
                {
                    if(!semicolon)
                    {
                        currCs.push_back(currString);
                    }
                    else
                    {
                        currString.erase(currString.size() - 1, 1);
                        currCs.push_back(currString);
                    }
                }
            }
            //Here, we detect the presence of connectors.
            bool detected = false;
            for(unsigned j = 0; j < currCs.size(); ++j)
            {
                if(currCs.at(j) == "||" || currCs.at(j) == "&&")
                {
                    detected = true;
                    break;
                }
            }
            //If it detects one, it sends the vector to a loop
            //that runs until the end of the current command string.
            //It stops at the next connector, looks at the previous connector,
            //and creates objects and links them correspondingly.
            if(detected)
            {
                string prevConnector;
                string nextConnector;
                vector<string> firstCommand;
                unsigned i = 0;
                while(currCs.at(i) != "&&" && currCs.at(i) != "||")
                {
                    firstCommand.push_back(currCs.at(i));
                    ++i;
                }
                prevConnector = currCs.at(i);
                Base * temp3;
                if(firstCommand.at(0) == "test")
                {
                    temp3 = new Test(firstCommand);
                }
                else
                {
                    temp3 = new Command(firstCommand);
                }
                currCmds.push_back(temp3);
                ++i;
                vector<string> currCommand;
                for(;i < currCs.size(); ++i)
                {
                    currCommand.push_back(currCs.at(i));
                    if(currCs.at(i) == "&&" || currCs.at(i) == "||")
                    {
                        currCommand.pop_back();
                        if(currCommand.at(0) == "test")
                        {
                            Base *temp = new Test(currCommand);
                            nextConnector = currCs.at(i);
                            if(prevConnector == "&&")
                            {
                                Base *temp2 = new Andand(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                            else
                            {
                                Base * temp2 = new Oror(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                        }
                        else {
                            Base *temp = new Command(currCommand);
                            nextConnector = currCs.at(i);
                            if(prevConnector == "&&")
                            {
                                Base *temp2 = new Andand(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                            else
                            {
                                Base * temp2 = new Oror(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                        }    
                    }
                }
            }
            //This runs if there no connectors left after semicolon detecting.
            else {
                vector<string> currCommand;
                for(unsigned k = 0; k < currCs.size(); ++k)
                {
                    currCommand.push_back(currCs.at(k));
                }
                Base *temp;
                if(currCommand.at(0) == "test")
                {
                    temp = new Test(currCommand);
                }
                else {
                    temp = new Command(currCommand);
                }
                currCmds.push_back(temp);
            }
        }
    }
    //Second loop that parses the remaining part of the input
    //Also, if the input never contained any semicolons,
    //the parse will go straight to this part of the code.
    currCs.resize(0);
    while(inSS >> currString)
    {
        if(currString.at(currString.size() - 1) == ';')
        {
            currString.erase(currString.size() - 1, 1);
        }
        string temp = "";
        for(unsigned g = 0; g < currString.size(); ++g)
        {
            if(currString.at(g) == '"')
            {
                if(g > 0 && currString.at(g - 1) == '\\')
                {
                    temp += currString.at(g);
                }
            }
            else if(currString.at(g) != '\\')
            {
                temp += currString.at(g);
            }
        }
        currString = temp;
        if(currString.find("#") != string::npos)
        {
            if(currString.find("#") != 0)
            {
                currCs.push_back(currString);
            }
            break;
        }
        currCs.push_back(currString);
    }
    //Again, detects whether the string has connectors.
    bool detected = false;
    for(unsigned j = 0; j < currCs.size(); ++j)
    {
        if(currCs.at(j) == "&&" || currCs.at(j) == "||")
        {
            detected = true;
            break;
        }
    }
    //If it detects them, it sends them to be parsed,
    //where connector objects are created after the first command is created
    //and subsequently linked until the end of parsing.
    if(detected)
    {
        string prevConnector;
        string nextConnector;
        vector<string> firstCommand;
        unsigned i = 0;
        while(currCs.at(i) != "&&" && currCs.at(i) != "||")
        {
            firstCommand.push_back(currCs.at(i));
            ++i;
        }
        prevConnector = currCs.at(i);
        Base *temp3;
        if(firstCommand.at(0) == "test")
        {
            temp3 = new Test(firstCommand);
        }
        else {
            temp3 = new Command(firstCommand);
        }
        currCmds.push_back(temp3);
        ++i;
        vector<string> currCommand;
        for(; i < currCs.size(); ++i)
        {
            currCommand.push_back(currCs.at(i));
            if(currCs.at(i) == "&&" || currCs.at(i) == "||")
            {
                currCommand.pop_back();
                Base *temp;
                if(currCommand.at(0) == "test")
                {
                    temp = new Test(currCommand);
                }
                else {
                    temp = new Command(currCommand);
                }
                nextConnector = currCs.at(i);
                if(prevConnector == "&&")
                {
                    Base *temp2 = new Andand(currCmds.at(currCmds.size() - 1), 
                        temp);
                    currCmds.pop_back();
                    currCmds.push_back(temp2);
                    prevConnector = nextConnector;
                    currCommand.resize(0);
                }
                else
                {
                    Base * temp2 = new Oror(currCmds.at(currCmds.size() - 1), 
                        temp);
                    currCmds.pop_back();
                    currCmds.push_back(temp2);
                    prevConnector = nextConnector;
                    currCommand.resize(0);
                }
            }
        }
        Base * temp;
        if(currCommand.at(0) == "test")
        {
            temp = new Test(currCommand);
        }
        else {
            temp = new Command(currCommand);
        }
        if(prevConnector == "&&")
            {
                Base *temp2 = new Andand(currCmds.at(currCmds.size() - 1), 
                    temp);
                currCmds.pop_back();
                currCmds.push_back(temp2);
                currCommand.resize(0);
            }
        else
        {
            Base * temp2 = new Oror(currCmds.at(currCmds.size() - 1), temp);
            currCmds.pop_back();
            currCmds.push_back(temp2);
        }
    }
    else {
        vector<string> currCommand;
        for(unsigned k = 0; k < currCs.size(); ++k)
        {
            currCommand.push_back(currCs.at(k));
        }
        Base *temp;
        if(currCommand.at(0) == "test")
        {
            temp = new Test(currCommand);
        }
        else {
            temp = new Command(currCommand);
        }
        currCmds.push_back(temp);
    }
}
//This is used to clean up the Processes vector in order to gather further
//input from the user.
void Processes::reset()
{
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        delete currCmds.at(i);
    }
    this->currCmds.resize(0);

}