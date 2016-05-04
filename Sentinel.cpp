#include <iostream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include "Sentinel.h"
using namespace std;

Sentinel::Sentinel()
{
    first = 0;
    second = 0;
}

Sentinel::Sentinel(Base* first, Base* second)
{
    this->first = first;
    this->second = second;
}

Sentinel::~Sentinel()
{
    delete first;
    delete second;
}