//DAVIDE MEROTTO

#include "../include/Device.h"

Device::Device(std::string nome, const int ID, double power) 
: name_{nome}, id_{ID}, power_consumption_{power}, status_{false}, power_used_{0} 
{
}

//Implementazione classe Devices

void Device::UpdatePowerUsed(const CustomTime& CurrentTime){
    power_used_ += power_consumption_*(CurrentTime - start_time_);
    start_time_ = CurrentTime;
}

void Device::Reset(){
    power_used_=0;
    status_ = false;
}

std::ostream& operator<<(std::ostream& os, Device& dev){
    if(dev.GetStatus()) {return os << dev.GetName() << " on " << dev.GetPowerUsed() ;}
    return os << dev.GetName() << " off " << dev.GetPowerUsed() ;
}