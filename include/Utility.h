#ifndef UTILITY_H
#define UTILITY_H

#include <iterator>
#include <utility> //per std::pair
#include <map>
#include "ManualDevice.h"
#include "AutomaticDevice.h"
#include "CustomTime.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

class Utility {
};

//trova device nella multimappa degli attivi
    std::multimap<CustomTime, Device*>::iterator findDevice(std::multimap<CustomTime, Device*> activeDevices, Device& d);
    //trova device nella multimappa degli attivi tramite ID
    std::multimap<CustomTime, Device*>::iterator findDeviceByID(std::multimap<CustomTime, Device*> activeDevices, int ID);
    //trova device nella multimappa degli attivi tramite nome
    std::multimap<CustomTime, Device*>::iterator findDeviceByNameActive(std::multimap<CustomTime, Device*> activeDevices, std::string const & s ) ;
    //trova device nella multimappa degli async tramite nome
    std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator findDeviceByNameAsync(std::multimap<CustomTime, std::pair<CustomTime, Device*>> asyncDevices, std::string const & s);
    //trova device nel vettore di tutti i device
    std::vector<Device*>::iterator findDeviceByNameAll(std::vector<Device*> deviceList, std::string const & s);

#endif