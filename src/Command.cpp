#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

#include "Base.h"
#include "Command.h"
//Runstat will be used by the command object in order to relay
//its execution status; that is, if it failed or not.
//It also has a vector that contains the operands of its command.
Command::Command()
{
    runStat = 0;
    cmdVec.resize(0);
}
//This constructor is similar to default, except that it directly
//initializes the cmdVec variable.
Command::Command(vector<string> currCommand)
{
    runStat = 0;
    cmdVec = currCommand;
}
//This is used by execute to tranform a s vector of strings to
//a char * array for use in execvp.
char *convert(const std::string & s)
{
   char *pc = new char[s.size()+1];
   std::strcpy(pc, s.c_str());
   return pc; 
}
//The execute function starts by converting the string vector to a
//char * array. Then, we fork a process to run a command.
//Because execvp only returns if it fails to execute a command,
//runStat gets updated if and only if the command fails.
//Finally, it clens up the dynamic allocation of the transformation process,
//and returns the runStat.
int Command::execute()
{
    bool pipetime = false;
    bool wepipes = false;
    int numpipes = 0;
    if(cmdVec.at(0) == "exit")
    {
        exit(0);
    }
    runStat = 0;
    for(unsigned j = 0; j < cmdVec.size(); ++j)
    {
        if(cmdVec.at(0) != "echo" && (cmdVec.at(j) == "<" || cmdVec.at(j) == ">>" || cmdVec.at(j) == ">" || cmdVec.at(j) == "|"))
        {
            pipetime = true;
        }
        if(cmdVec.at(j) == "|")
        {
            wepipes = true;
            ++numpipes;
        }
    }
    if(pipetime)
    {
        return redirection(wepipes, numpipes);
    }
    char** cstrings = new char*[cmdVec.size() + 1];
    unsigned i = 0;
    for(; i < this->cmdVec.size(); ++i)
    {
        cstrings[i] = new char[cmdVec[i].size() + 1];
        std::strcpy(cstrings[i], cmdVec[i].c_str());
    }
    cstrings[i] = NULL;
    pid_t pid = fork();
    if(pid == 0)
    {
        //Child process!
        runStat = execvp(cstrings[0], cstrings);
        if(runStat != 0)
        {
            perror("Command has failed to execute");
            return -1;
        }
    }
    int status;
    waitpid(pid, &status, 0);
    for(unsigned i = 0; i < cmdVec.size(); ++i)
    {
        delete[] cstrings[i];
    }
    delete[] cstrings;
    return status;
}
//Simple accessor to access the runStat.
int Command::getrunstat()
{
    return runStat;
}
//Runs commands that have piping/input redirection/output redirection
int Command::redirection(bool pipes, int numPipes)
{
    //Setting up a bunch of proxy string vectors
    vector<unsigned> indices;
    vector<vector<string> > cmds;
    vector<string> temp;
    //This is to initialize a 2d array of char arrays for piping purposes
    for(unsigned h = 0; h < cmdVec.size(); ++h)
    {
        temp.push_back(cmdVec.at(h));
        if(cmdVec.at(h) == "|")
        {
            temp.pop_back();
            cmds.push_back(temp);
            temp.resize(0);
        }
    }
    //Hear we test the existence of redirection operators by using boolean values
    cmds.push_back(temp);
    bool inputread = false;
    bool overwrite = false;
    bool append = false;
    for(unsigned i = 0; i < cmdVec.size(); ++i)
    {
        if(cmdVec.at(i) == "<")
        {
            inputread = true;
        }
        if(cmdVec.at(i) == ">")
        {
            overwrite = true;
        }
        if(cmdVec.at(i) == ">>")
        {
            append = true;
        }
    }
    //This is creating the 2d array that's needed for piping purposes
    char*** ary = new char**[cmds.size()];
    for(unsigned g = 0; g < cmds.size(); ++g)
    {
        char** cstrings = new char*[cmds.at(g).size() + 1];
        unsigned i = 0;
        for(; i < cmds.at(g).size(); ++i)
        {
            cstrings[i] = new char[cmds.at(g)[i].size() + 1];
            std::strcpy(cstrings[i], cmds.at(g).at(i).c_str());
        }
        cstrings[i] = NULL;
        ary[g] = cstrings;
    }
    //Here's where things get tricky; we're testing for each case of i/o redirection, s we use our boolean values to check
    //But, we also need to know whether these operators exist without the additional existence of pipes, so we need an if/else insdie each check
    //This covers < and > together
    if(inputread && overwrite && !append)
    {
        if(pipes)
        {
            //Here we set up the variables needed for duping, piping, and indices for determining the index of our files
            //in the vector of strings
            int in, out;
            int status;
            unsigned indexforless = 0;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == "<")
                {
                    indexforless = i;
                }
                if(cmdVec.at(i) == ">")
                {
                    indexforover = i;
                }
            }
            //Create an array for the filename
            char** cstrings = new char*[indexforless + 1];
            unsigned m = 0;
            for(; m < indexforless; ++m)
            {
                cstrings[m] = new char[cmdVec[m].size() + 1];
                std::strcpy(cstrings[m], cmdVec[m].c_str());
            }
            cstrings[m] = NULL;
            vector<string> temp;
            unsigned c = 0;
            for(; c < cmds.at(cmds.size() - 1).size(); ++c)
            {
                temp.push_back(cmds.at(cmds.size() - 1).at(c));
                if(cmds.at(cmds.size() - 1).at(c) == ">")
                {
                    break;
                }
            }
            //This is for the last part of the string vector, the output file
            temp.pop_back();
            char** cstrings4 = new char*[temp.size() + 1];
            unsigned x = 0;
            for(; x < temp.size(); ++x)
            {
                cstrings4[x] = new char[temp.at(x).size() + 1];
                std::strcpy(cstrings4[x], temp.at(x).c_str());
            }
            cstrings4[x] = NULL;
            char** cstrings2 = new char*[1];
            char** cstrings3 = new char*[1];
            
            cstrings2[0] = new char[cmdVec.at(indexforless + 1).size() + 1];
            std::strcpy(cstrings2[0], cmdVec[indexforless + 1].c_str());
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            //Now we create our pipes based on how many we initially passed in
            int i = 0;
            pid_t pid;
            int g = 2 * numPipes;
            int* pipefds = new int[g];
        
            for(i = 0; i < (numPipes); i++){
                if(pipe(pipefds + i*2) < 0) {
                    perror("Couldn't pipe");
                    return -1;
                }
            }
            //This loop forks processes inside to properly encapsulate the algorithm for duping and executing various commands
            //It sets up the file input/output dupes at the beginning and end, and handles the rest by duping normally
            unsigned b = 0;
            int j = 0;
            while(b < cmds.size()) {
                pid = fork();
                if(pid == 0) {
                    //if not last command
                    if(b < cmds.size() - 1){
                        if(b == 0)
                        {
                            in = open(cstrings2[0], O_RDONLY);
                            dup2(in, 0);
                            close(in);
                        }
                        if(dup2(pipefds[j + 1], 1) < 0){
                            perror("Problem with dup2");
                            return -1;
                        }
                    }
                    //if not first command && j != 2*numPipes
                    if(j != 0 ){
                        if(dup2(pipefds[j - 2], 0) < 0){
                            perror("Problem with dup2");///j-2 0 j+1 1
                            return -1;
                        }
                        if(b == cmds.size() - 1)
                        {
                            out = open(cstrings3[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                            dup2(out, 1);
                            close(out);
                        }
                    }
                    for(i = 0; i < 2*numPipes; i++){
                        close(pipefds[i]);
                    }
                    if(b > 0 && b < cmds.size() - 1) {
                        if(execvp(ary[b][0], ary[b]) < 0){
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                    else if(b == 0)
                    {
                        if(execvp(cstrings[0], cstrings) < 0)
                        {
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                    else
                    {
                        if(execvp(cstrings4[0], cstrings4) < 0)
                        {
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                } else if(pid < 0){
                    perror("Error forking");
                    return -1;
                }
                ++b;
                j += 2;
            }
            /**Parent closes the pipes and wait for children*/
            for(i = 0; i < 2 * numPipes; i++){
                close(pipefds[i]);
            }
            for(i = 0; i < numPipes + 1; i++)
            {
                wait(&status);
            }
        }
        //This is a much simpler clause; we don't need to use pipes or loop in one,
        //so we can simply dupe i/o file descriptors and execute according based on our created arrays
        else
        {
            int in, out;
            int status;
            pid_t pid = 0;
            unsigned indexforless = 0;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == "<")
                {
                    indexforless = i;
                }
                if(cmdVec.at(i) == ">")
                {
                    indexforover = i;
                }
            }
            char** cstrings = new char*[indexforless + 1];
            unsigned i = 0;
            for(; i < indexforless; ++i)
            {
                cstrings[i] = new char[cmdVec[i].size() + 1];
                std::strcpy(cstrings[i], cmdVec[i].c_str());
            }
            cstrings[i] = NULL;
            pid = fork();
            char** cstrings2 = new char*[1];
            char** cstrings3 = new char*[1];
            
            cstrings2[0] = new char[cmdVec.at(indexforless + 1).size() + 1];
            std::strcpy(cstrings2[0], cmdVec[indexforless + 1].c_str());
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            if(pid == 0)
            {
                in = open(cstrings2[0], O_RDONLY);
                out = open(cstrings3[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                // replace standard input with input file
                dup2(in, 0);
                // replace standard output with output file
                dup2(out, 1);
                // close unused file descriptors
                close(in);
                close(out);
                runStat = execvp(cstrings[0], cstrings);
                if(runStat != 0)
                {
                    perror("Execvp error");
                }
            }
            waitpid(pid, &status, 0);
            delete cstrings2[0];
            delete cstrings3[0];
            delete [] cstrings2;
            delete [] cstrings3;
            for(unsigned f = 0; f < i; ++f)
            {
                delete cstrings[f];
            }
            delete [] cstrings;
            return status;
        }
    }
    //Think of the rest of these as variations on the previously coded algorithm for determining pipes, i/o duping etc.
    //Append only changes a flag that disables truncating and enables appending to a file
    else if(inputread && !overwrite && append)
    {
        if(pipes)
        {
            int in, out;
            int status;
            unsigned indexforless = 0;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == "<")
                {
                    indexforless = i;
                }
                if(cmdVec.at(i) == ">")
                {
                    indexforover = i;
                }
            }
            char** cstrings = new char*[indexforless + 1];
            unsigned m = 0;
            for(; m < indexforless; ++m)
            {
                cstrings[m] = new char[cmdVec[m].size() + 1];
                std::strcpy(cstrings[m], cmdVec[m].c_str());
            }
            cstrings[m] = NULL;
            vector<string> temp;
            unsigned c = 0;
            for(; c < cmds.at(cmds.size() - 1).size(); ++c)
            {
                temp.push_back(cmds.at(cmds.size() - 1).at(c));
                if(cmds.at(cmds.size() - 1).at(c) == ">")
                {
                    break;
                }
            }
            temp.pop_back();
            char** cstrings4 = new char*[temp.size() + 1];
            unsigned x = 0;
            for(; x < temp.size(); ++x)
            {
                cstrings4[x] = new char[temp.at(x).size() + 1];
                std::strcpy(cstrings4[x], temp.at(x).c_str());
            }
            cstrings4[x] = NULL;
            char** cstrings2 = new char*[1];
            char** cstrings3 = new char*[1];
            
            cstrings2[0] = new char[cmdVec.at(indexforless + 1).size() + 1];
            std::strcpy(cstrings2[0], cmdVec[indexforless + 1].c_str());
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            int i = 0;
            pid_t pid;
            int g = 2 * numPipes;
            int* pipefds = new int[g];
        
            for(i = 0; i < (numPipes); i++){
                if(pipe(pipefds + i*2) < 0) {
                    perror("Couldn't pipe");
                    return -1;
                }
            }
            unsigned b = 0;
            int j = 0;
            while(b < cmds.size()) {
                pid = fork();
                if(pid == 0) {
                    //if not last command
                    if(b < cmds.size() - 1){
                        if(b == 0)
                        {
                            in = open(cstrings2[0], O_RDONLY);
                            dup2(in, 0);
                            close(in);
                        }
                        if(dup2(pipefds[j + 1], 1) < 0){
                            perror("Problem with dup2");
                            return -1;
                        }
                    }
                    //if not first command && j != 2*numPipes
                    if(j != 0 ){
                        if(dup2(pipefds[j - 2], 0) < 0){
                            perror("Problem with dup2");///j-2 0 j+1 1
                            return -1;
                        }
                        if(b == cmds.size() - 1)
                        {
                            out = open(cstrings3[0], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                            dup2(out, 1);
                            close(out);
                        }
                    }
                    for(i = 0; i < 2*numPipes; i++){
                        close(pipefds[i]);
                    }
                    if(b > 0 && b < cmds.size() - 1) {
                        if(execvp(ary[b][0], ary[b]) < 0){
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                    else if(b == 0)
                    {
                        if(execvp(cstrings[0], cstrings) < 0)
                        {
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                    else
                    {
                        if(execvp(cstrings4[0], cstrings4) < 0)
                        {
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                } else if(pid < 0){
                    perror("Error forking");
                    return -1;
                }
                ++b;
                j += 2;
            }
            /**Parent closes the pipes and wait for children*/
            for(i = 0; i < 2 * numPipes; i++){
                close(pipefds[i]);
            }
            for(i = 0; i < numPipes + 1; i++)
            {
                wait(&status);
            }
        }
        else
        {
            int in, out;
            int status;
            pid_t pid = 0;
            unsigned indexforless = 0;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == "<")
                {
                    indexforless = i;
                }
                if(cmdVec.at(i) == ">")
                {
                    indexforover = i;
                }
            }
            char** cstrings = new char*[indexforless + 1];
            unsigned i = 0;
            for(; i < indexforless; ++i)
            {
                cstrings[i] = new char[cmdVec[i].size() + 1];
                std::strcpy(cstrings[i], cmdVec[i].c_str());
            }
            cstrings[i] = NULL;
            pid = fork();
            char** cstrings2 = new char*[1];
            char** cstrings3 = new char*[1];
            
            cstrings2[0] = new char[cmdVec.at(indexforless + 1).size() + 1];
            std::strcpy(cstrings2[0], cmdVec[indexforless + 1].c_str());
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            if(pid == 0)
            {
                in = open(cstrings2[0], O_RDONLY);
                out = open(cstrings3[0], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                // replace standard input with input file
                dup2(in, 0);
                // replace standard output with output file
                dup2(out, 1);
                // close unused file descriptors
                close(in);
                close(out);
                runStat = execvp(cstrings[0], cstrings);
                if(runStat != 0)
                {
                    perror("Execvp error");
                }
            }
            waitpid(pid, &status, 0);
            delete cstrings2[0];
            delete cstrings3[0];
            delete [] cstrings2;
            delete [] cstrings3;
            for(unsigned f = 0; f < i; ++f)
            {
                delete cstrings[f];
            }
            delete [] cstrings;
            return status;
        }
    }
    //Here we make even simpler versions of our original alogrithm;
    //These are cases where just one of the operators are detected, so only one variation is made on
    //either the stat our end of duping in our piping loop;
    //The else statements are even easier
    else if(inputread && !overwrite && !append)
    {
        if(pipes)
        {
            int in;
            int status;
            unsigned indexforless = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == "<")
                {
                    indexforless = i;
                }
            }
            char** cstrings = new char*[indexforless + 1];
            unsigned m = 0;
            for(; m < indexforless; ++m)
            {
                cstrings[m] = new char[cmdVec[m].size() + 1];
                std::strcpy(cstrings[m], cmdVec[m].c_str());
            }
            cstrings[m] = NULL;
            char** cstrings2 = new char*[1];
            
            cstrings2[0] = new char[cmdVec.at(indexforless + 1).size() + 1];
            std::strcpy(cstrings2[0], cmdVec[indexforless + 1].c_str());
            int i = 0;
            pid_t pid;
            int g = 2 * numPipes;
            int* pipefds = new int[g];
        
            for(i = 0; i < (numPipes); i++){
                if(pipe(pipefds + i*2) < 0) {
                    perror("Couldn't pipe");
                    return -1;
                }
            }
            unsigned b = 0;
            int j = 0;
            while(b < cmds.size()) {
                pid = fork();
                if(pid == 0) {
                    //if not last command
                    if(b < cmds.size() - 1){
                        if(b == 0)
                        {
                            in = open(cstrings2[0], O_RDONLY);
                            dup2(in, 0);
                            close(in);
                        }
                        if(dup2(pipefds[j + 1], 1) < 0){
                            perror("Problem with dup2");
                            return status;
                        }
                    }
                    //if not first command && j != 2*numPipes
                    if(j != 0 ){
                        if(dup2(pipefds[j - 2], 0) < 0){
                            perror("Problem with dup2");///j-2 0 j+1 1
                            return status;
                        }
                    }
                    for(i = 0; i < 2*numPipes; i++){
                        close(pipefds[i]);
                    }
                    if(b > 0) {
                        if(execvp(ary[b][0], ary[b]) < 0){
                            perror("Execvp problem");
                            return status;
                        }
                    }
                    else if(b == 0)
                    {
                        if(execvp(cstrings[0], cstrings) < 0)
                        {
                            perror("Execvp problem");
                            return status;
                        }
                    }
                } else if(pid < 0){
                    perror("Error forking");
                    return status;
                }
                ++b;
                j += 2;
            }
            /**Parent closes the pipes and wait for children*/
            for(i = 0; i < 2 * numPipes; i++){
                close(pipefds[i]);
            }
            for(i = 0; i < numPipes + 1; i++)
            {
                wait(&status);
            }
        }
        else
        {
            int in;
            int status;
            pid_t pid = 0;
            unsigned indexforless = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == "<")
                {
                    indexforless = i;
                }
            }
            char** cstrings = new char*[indexforless + 1];
            unsigned i = 0;
            for(; i < indexforless; ++i)
            {
                cstrings[i] = new char[cmdVec[i].size() + 1];
                std::strcpy(cstrings[i], cmdVec[i].c_str());
            }
            cstrings[i] = NULL;
            pid = fork();
            char** cstrings2 = new char*[1];
            
            cstrings2[0] = new char[cmdVec.at(indexforless + 1).size() + 1];
            std::strcpy(cstrings2[0], cmdVec[indexforless + 1].c_str());
            if(pid == 0)
            {
                in = open(cstrings2[0], O_RDONLY);
                // replace standard input with input file
                dup2(in, 0);
                // replace standard output with output file
                // close unused file descriptors
                close(in);
                runStat = execvp(cstrings[0], cstrings);
                if(runStat != 0)
                {
                    perror("Execvp error");
                }
            }
            waitpid(pid, &status, 0);
            delete cstrings2[0];
            delete [] cstrings2;
            for(unsigned f = 0; f < i; ++f)
            {
                delete cstrings[f];
            }
            delete [] cstrings;
            return status;
        }
    }
    else if(!inputread && overwrite && !append)
    {
        if(pipes)
        {
            int out;
            int status;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == ">")
                {
                    indexforover = i;
                }
            }
            vector<string> temp;
            unsigned c = 0;
            for(; c < cmds.at(cmds.size() - 1).size(); ++c)
            {
                temp.push_back(cmds.at(cmds.size() - 1).at(c));
                if(cmds.at(cmds.size() - 1).at(c) == ">" || cmds.at(cmds.size() - 1).at(c) == ">>")
                {
                    break;
                }
            }
            temp.pop_back();
            char** cstrings4 = new char*[temp.size() + 1];
            unsigned x = 0;
            for(; x < temp.size(); ++x)
            {
                cstrings4[x] = new char[temp.at(x).size() + 1];
                std::strcpy(cstrings4[x], temp.at(x).c_str());
            }
            cstrings4[x] = NULL;
            char** cstrings3 = new char*[1];
            
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            int i = 0;
            pid_t pid;
            int g = 2 * numPipes;
            int* pipefds = new int[g];
        
            for(i = 0; i < (numPipes); i++){
                if(pipe(pipefds + i*2) < 0) {
                    perror("Couldn't pipe");
                    return -1;
                }
            }
            unsigned b = 0;
            int j = 0;
            while(b < cmds.size()) {
                pid = fork();
                if(pid == 0) {
                    //if not last command
                    if(b < cmds.size() - 1){
                        if(dup2(pipefds[j + 1], 1) < 0){
                            perror("Problem with dup2");
                            return -1;
                        }
                    }
                    //if not first command && j != 2*numPipes
                    if(j != 0 ){
                        if(dup2(pipefds[j - 2], 0) < 0){
                            perror("Problem with dup2");///j-2 0 j+1 1
                            return -1;
                        }
                        if(b == cmds.size() - 1)
                        {
                            out = open(cstrings3[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                            dup2(out, 1);
                            close(out);
                        }
                    }
                    for(i = 0; i < 2*numPipes; i++){
                        close(pipefds[i]);
                    }
                    if(b < cmds.size() - 1) {
                        if(execvp(ary[b][0], ary[b]) < 0){
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                    else
                    {
                        if(execvp(cstrings4[0], cstrings4) < 0)
                        {
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                } else if(pid < 0){
                    perror("Error forking");
                    return -1;
                }
                ++b;
                j += 2;
            }
            /**Parent closes the pipes and wait for children*/
            for(i = 0; i < 2 * numPipes; i++){
                close(pipefds[i]);
            }
            for(i = 0; i < numPipes + 1; i++)
            {
                wait(&status);
            }
        }
        else
        {
            int out;
            int status;
            pid_t pid = 0;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == ">")
                {
                    indexforover = i;
                }
            }
            char** cstrings = new char*[indexforover + 1];
            unsigned i = 0;
            for(; i < indexforover; ++i)
            {
                cstrings[i] = new char[cmdVec[i].size() + 1];
                std::strcpy(cstrings[i], cmdVec[i].c_str());
            }
            cstrings[i] = NULL;
            pid = fork();
            char** cstrings3 = new char*[1];
            
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            if(pid == 0)
            {
                out = open(cstrings3[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                // replace standard output with output file
                dup2(out, 1);
                // close unused file descriptors
                close(out);
                runStat = execvp(cstrings[0], cstrings);
                if(runStat != 0)
                {
                    perror("Execvp error");
                }
            }
            waitpid(pid, &status, 0);
            delete cstrings3[0];
            delete [] cstrings3;
            for(unsigned f = 0; f < i; ++f)
            {
                delete cstrings[f];
            }
            delete [] cstrings;
            return status;
        }
    }
    else if(!inputread && !overwrite && append)
    {
        if(pipes)
        {
            int out;
            int status;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == ">>")
                {
                    indexforover = i;
                }
            }
            vector<string> temp;
            unsigned c = 0;
            for(; c < cmds.at(cmds.size() - 1).size(); ++c)
            {
                temp.push_back(cmds.at(cmds.size() - 1).at(c));
                if(cmds.at(cmds.size() - 1).at(c) == ">" || cmds.at(cmds.size() - 1).at(c) == ">>")
                {
                    break;
                }
            }
            temp.pop_back();
            char** cstrings4 = new char*[temp.size() + 1];
            unsigned x = 0;
            for(; x < temp.size(); ++x)
            {
                cstrings4[x] = new char[temp.at(x).size() + 1];
                std::strcpy(cstrings4[x], temp.at(x).c_str());
            }
            cstrings4[x] = NULL;
            char** cstrings3 = new char*[1];
            
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            int i = 0;
            pid_t pid;
            int g = 2 * numPipes;
            int* pipefds = new int[g];
        
            for(i = 0; i < (numPipes); i++){
                if(pipe(pipefds + i*2) < 0) {
                    perror("Couldn't pipe");
                    return -1;
                }
            }
            unsigned b = 0;
            int j = 0;
            while(b < cmds.size()) {
                pid = fork();
                if(pid == 0) {
                    //if not last command
                    if(b < cmds.size() - 1){
                        if(dup2(pipefds[j + 1], 1) < 0){
                            perror("Problem with dup2");
                            return -1;
                        }
                    }
                    //if not first command && j != 2*numPipes
                    if(j != 0 ){
                        if(dup2(pipefds[j - 2], 0) < 0){
                            perror("Problem with dup2");///j-2 0 j+1 1
                            return -1;
                        }
                        if(b == cmds.size() - 1)
                        {
                            out = open(cstrings3[0], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                            dup2(out, 1);
                            close(out);
                        }
                    }
                    for(i = 0; i < 2*numPipes; i++){
                        close(pipefds[i]);
                    }
                    if(b < cmds.size() - 1) {
                        if(execvp(ary[b][0], ary[b]) < 0){
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                    else
                    {
                        if(execvp(cstrings4[0], cstrings4) < 0)
                        {
                            perror("Execvp problem");
                            return -1;
                        }
                    }
                } else if(pid < 0){
                    perror("Error forking");
                    return -1;
                }
                ++b;
                j += 2;
            }
            /**Parent closes the pipes and wait for children*/
            for(i = 0; i < 2 * numPipes; i++){
                close(pipefds[i]);
            }
            for(i = 0; i < numPipes + 1; i++)
            {
                wait(&status);
            }
        }
        else
        {
            int out;
            int status;
            pid_t pid = 0;
            unsigned indexforover = 0;
            for(unsigned i = 0; i < cmdVec.size(); ++i)
            {
                if(cmdVec.at(i) == ">>")
                {
                    indexforover = i;
                }
            }
            char** cstrings = new char*[indexforover + 1];
            unsigned i = 0;
            for(; i < indexforover; ++i)
            {
                cstrings[i] = new char[cmdVec[i].size() + 1];
                std::strcpy(cstrings[i], cmdVec[i].c_str());
            }
            cstrings[i] = NULL;
            pid = fork();
            char** cstrings3 = new char*[1];
            
            cstrings3[0] = new char[cmdVec.at(indexforover + 1).size() + 1];
            std::strcpy(cstrings3[0], cmdVec[indexforover + 1].c_str());
            if(pid == 0)
            {
                out = open(cstrings3[0], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                // replace standard output with output file
                dup2(out, 1);
                // close unused file descriptors
                close(out);
                runStat = execvp(cstrings[0], cstrings);
                if(runStat != 0)
                {
                    perror("Execvp error");
                }
            }
            waitpid(pid, &status, 0);
            delete cstrings3[0];
            delete [] cstrings3;
            for(unsigned f = 0; f < i; ++f)
            {
                delete cstrings[f];
            }
            delete [] cstrings;
            return status;
        }
    }
    //This last else statement handles the case when no i/o redirection operators are needed to be dealt with;
    //It just simply creates a number of pipes based on the passed in value, and executes each command in the pipeline
    else
    {
        int status;
        int i = 0;
        pid_t pid;
        int g = 2 * numPipes;
        int* pipefds = new int[g];
    
        for(i = 0; i < (numPipes); i++){
            if(pipe(pipefds + i*2) < 0) {
                perror("Couldn't pipe");
                return -1;
            }
        }
        unsigned b = 0;
        int j = 0;
        while(b < cmds.size()) {
            pid = fork();
            if(pid == 0) {
                //if not last command
                if(b < cmds.size() - 1){
                    if(dup2(pipefds[j + 1], 1) < 0){
                        perror("Problem with dup2");
                        return -1;
                    }
                }
                //if not first command&& j!= 2*numPipes
                if(j != 0 ){
                    if(dup2(pipefds[j - 2], 0) < 0){
                        perror("Problem with dup2");///j-2 0 j+1 1
                        return -1;
                    }
                }
                for(i = 0; i < 2*numPipes; i++){
                    close(pipefds[i]);
                }
    
                if(execvp(ary[b][0], ary[b]) < 0 ){
                    perror("Execvp problem");
                    return -1;
                }
            } else if(pid < 0){
                perror("Error forking");
                return -1;
            }
            ++b;
            j += 2;
        }
        /**Parent closes the pipes and wait for children*/
        for(i = 0; i < 2 * numPipes; i++){
            close(pipefds[i]);
        }
        for(i = 0; i < numPipes + 1; i++)
        {
            wait(&status);
        }
    }
    return 0;
}