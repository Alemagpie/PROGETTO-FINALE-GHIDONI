#ifndef DEV_MANAGER_H
#define DEV_MANAGER_H

#include <iterator>
#include <map>
#include "Device.h"
#include "Time.h"
#include <sstream>
#include <iostream>

//albero dei dispositivi
class DeviceManager {
    public:
    DeviceManager();

    private:
    std::multimap<Time, Device> activeDevices;
    std::multimap<Time, Device> asyncDevices;
    Time currentTime;
    Time currentDeviceEndTime;

    void addDevice(Device& d);
    void addDeviceAsync(Device& d);
    Device getDevice(Time t);
    Device removeDevice(Time t);
    void parseInput(std::string command);
    void setTime(Time& newTime);
};

#endif