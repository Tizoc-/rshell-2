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
#include "Command.h"
#include "Processes.h"
#include "Sentinel.h"
#include "Andand.h"
#include "Oror.h"
using namespace std;

int main()
{
    string input;
    cout << "$ ";
    getline(cin, input);
    Proccesses mainProcess;
    while(input != "exit")
    {
        mainProcess.parse(input);
        mainProcess.execute();
        cout << "$ ";
        getline(cin, input);
    }
    return 0;
}