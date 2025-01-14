//ALESSIO FAGNANI

#include "../include/Utility.h"

#include <iterator>
#include <utility> //per std::pair
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "../include/ManualDevice.h"
#include "../include/AutomaticDevice.h"
#include "../include/CustomTime.h"

std::multimap<CustomTime, Device*>::iterator utility::FindDevice(std::multimap<CustomTime, Device*>& activeDevices,const Device& d) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&d](const std::pair<CustomTime, Device*>& element) -> bool {
            return *(element.second) == d;
        });
}

std::multimap<CustomTime, Device*>::iterator utility::FindDeviceByID(std::multimap<CustomTime, Device*>& activeDevices, int ID) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&ID](const std::pair<CustomTime, Device*>& element) -> bool {
            return element.second->GetID() == ID;
        });
}

std::multimap<CustomTime, Device*>::iterator utility::FindDeviceByNameActive(std::multimap<CustomTime, Device*>& activeDevices, const std::string & s ) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&s](const std::pair<CustomTime, Device*>& element) -> bool {
            return element.second->GetName() == s;
        });
}

std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator utility::FindDeviceByNameAsync(std::multimap<CustomTime, std::pair<CustomTime, Device*>>& asyncDevices, const std::string & s) {
    return std::find_if(asyncDevices.begin(), asyncDevices.end(),
        [&s](const std::pair<CustomTime, std::pair<CustomTime, Device*>>& element) -> bool {
            return element.second.second->GetName() == s;
        });
}

std::vector<Device*>::iterator utility::FindDeviceByNameAll(std::vector<Device*>& deviceList, const std::string & s) {
    return std::find_if(deviceList.begin(), deviceList.end(),
        [&s](Device* d) -> bool {
            return d->GetName() == s;
        });
}