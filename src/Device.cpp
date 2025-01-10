//DAVIDE MEROTTO

#include "../include/Device.h"

Device::Device(std::string nome, const int ID, double power) 
: name{nome}, id{ID}, powerConsumption{power}, status{false}, powerUsed{0} 
{
}

//Implementazione classe Devices
void Device:: stopDevice(){ 
    status=false;
}

void Device:: startDevice(){                
    status=true;
}

std::string Device:: getName() const { 
    return name;
}

int Device:: getID() const { 
    return id;
}

bool Device:: getStatus() const { 
    return status;
}

CustomTime Device::getStartTime() const{
    return start_time;
}

CustomTime Device::getEndTime() const{
    return end_time;
}

const double Device::getCurrentPowerConsumption() {
    return powerConsumption;
}

const double Device::getPowerUsed(){
    //si da per scontato che il dispositvo sia acceso
    return powerUsed;
}

void Device:: setStatus(bool value){
    status = value;
}

void Device::updatePowerUsed(CustomTime& currTime){
    powerUsed += powerConsumption*(currTime - start_time);
}

bool operator==(const Device& a, const Device& b) {
    return (a.getName() == b.getName() && a.getID() == b.getID());
}

std::ostream& operator<<(std::ostream& os, Device& dev){
    if(dev.getStatus()) {return os << dev.getName() << " on " << dev.getPowerUsed() ;}
    return os << dev.getName() << " off " << dev.getPowerUsed() ;
}