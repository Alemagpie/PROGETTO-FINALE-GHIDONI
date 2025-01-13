//ALESSIO FAGNANI

#include "../include/Utility.h"

std::multimap<CustomTime, Device*>::iterator Utility::findDevice(std::multimap<CustomTime, Device*>& activeDevices, Device& d) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&d](const std::pair<CustomTime, Device*>& element) -> bool {
            return *(element.second) == d;
        });
}

std::multimap<CustomTime, Device*>::iterator Utility::findDeviceByID(std::multimap<CustomTime, Device*>& activeDevices, int ID) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&ID](const std::pair<CustomTime, Device*>& element) -> bool {
            return element.second->getID() == ID;
        });
}

std::multimap<CustomTime, Device*>::iterator Utility::findDeviceByNameActive(std::multimap<CustomTime, Device*>& activeDevices, std::string const & s ) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&s](const std::pair<CustomTime, Device*>& element) -> bool {
            return element.second->getName() == s;
        });
}

std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator Utility::findDeviceByNameAsync(std::multimap<CustomTime, std::pair<CustomTime, Device*>>& asyncDevices, std::string const & s) {
    return std::find_if(asyncDevices.begin(), asyncDevices.end(),
        [&s](const std::pair<CustomTime, std::pair<CustomTime, Device*>>& element) -> bool {
            return element.second.second->getName() == s;
        });
}

std::vector<Device*>::iterator Utility::findDeviceByNameAll(std::vector<Device*>& deviceList, std::string const & s) {
    return std::find_if(deviceList.begin(), deviceList.end(),
        [&s](Device* d) -> bool {
            return d->getName() == s;
        });
}