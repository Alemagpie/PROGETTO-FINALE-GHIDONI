#ifndef CONSOLE_H
#define CONSOLE_H

#include "DeviceManager.h"
#include <string>

class Console{
    Console(DeviceManager& d);
    void parseInput(std::string);
    
};

#endif