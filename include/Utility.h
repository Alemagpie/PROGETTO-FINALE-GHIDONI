//ALESSIO FAGNANI

#ifndef UTILITY_H
#define UTILITY_H

#include <iterator>
#include <utility> //per std::pair
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "ManualDevice.h"
#include "AutomaticDevice.h"
#include "CustomTime.h"

namespace utility{
    //trova device nella multimappa degli attivi
    std::multimap<CustomTime, Device*>::iterator FindDevice(std::multimap<CustomTime, Device*>& activeDevices,const Device& d);
    //trova device nella multimappa degli attivi tramite ID
    std::multimap<CustomTime, Device*>::iterator FindDeviceByID(std::multimap<CustomTime, Device*>& activeDevices, int ID);
    //trova device nella multimappa degli attivi tramite nome
    std::multimap<CustomTime, Device*>::iterator FindDeviceByNameActive(std::multimap<CustomTime, Device*>& activeDevices, const std::string const & s ) ;
    //trova device nella multimappa degli async tramite nome
    std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator FindDeviceByNameAsync(std::multimap<CustomTime, std::pair<CustomTime, Device*>>& asyncDevices, const std::string  & s);
    //trova device nel vettore di tutti i device
    std::vector<Device*>::iterator FindDeviceByNameAll(std::vector<Device*>& deviceList, const std::string & s);
} //namespace Utility

#endif