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
#include <pwd.h>
#include <sys/types.h>

using namespace std;

#include "Processes.h"
//The main control interface. All main does is
//create a process object in order to keep the program running and working.
int main()
{
    
    struct passwd *pwd;
    pwd = getpwuid(geteuid());
    cout << pwd->pw_name;
    cout << "$ ";
    string input;
    getline(cin, input);
    Processes mainProcess;
    while(input != "exit")
    {
        mainProcess.parse(input);
        mainProcess.execute();
        mainProcess.reset();
        input = "";
        cout << "$ ";
        getline(cin, input);
    }
    return 0;
}