//DAVIDE MEROTTO

#include "../include/Device.h"

Device::Device(std::string nome, const int ID, double power) 
: name{nome}, id{ID}, powerConsumption{power}, status{false}, powerUsed{0} 
{
}

//Implementazione classe Devices

void Device::updatePowerUsed(CustomTime& currTime){
    powerUsed += powerConsumption*(currTime - start_time);
    start_time = currTime;
}

void Device::reset(){
    powerUsed=0;
    status = false;
}

std::ostream& operator<<(std::ostream& os, Device& dev){
    if(dev.getStatus()) {return os << dev.getName() << " on " << dev.getPowerUsed() ;}
    return os << dev.getName() << " off " << dev.getPowerUsed() ;
}