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
//Runs commands that have piping/input redirection/outputredirection
int Command::redirection(bool pipes, int numPipes)
{
    vector<unsigned> indices;
    vector<vector<string> > cmds;
    vector<string> temp;
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
    if(inputread && overwrite && !append)
    {
        if(pipes)
        {
            
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
    else if(inputread && !overwrite && append)
    {
        if(pipes)
        {
            
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
    else if(inputread && !overwrite && !append)
    {
        if(pipes)
        {
            
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
                    if(dup2(pipefds[j-2], 0) < 0){
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