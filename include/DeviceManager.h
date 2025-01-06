#ifndef DEV_MANAGER_H
#define DEV_MANAGER_H

#include <iterator>
#include <map>
#include "Device.h"
#include "Time.h"

//albero dei dispositivi
class DeviceManager {
    public:
    void addDevice(Device& d);
    Device getDevice(Time t);
    Device removeDevice(Time t);

    private:
    std::multimap<Time, Device> activeDevices;
    std::multimap<Time, Device> asyncDevices;
};

#endif