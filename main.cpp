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
using namespace std;

#include "Processes.h"

int main()
{
    cout << "$ ";
    string input;
    getline(cin, input);
    Processes mainProcess;
    while(input != "exit")
    {
        mainProcess.parse(input);
        mainProcess.execute();
        cout << "$ ";
        getline(cin, input);
    }
    return 0;
}