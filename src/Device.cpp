//DAVIDE MEROTTO

#include "../include/Device.h"

Device::Device(std::string nome, const int ID, double en) 
: name{nome}, id{ID}, energyUse{en}, status{false} {
    start_time.setTime(0,0);
    end_time.setTime(0,0);
}

//Implementazione classe Devices
void Device:: stopDevice(){ 
    status=false;
}

void Device:: startDevice(){                
    status=true;
}

std::string Device:: getName(){ 
    return name;
}

const int Device:: getID(){ 
    return id;
}

bool Device:: getStatus(){ 
    return status;
}

Time Device::getEndTime() {
    return end_time;
}

double Device:: getEnergyUseByTime(Time& current_time){
    
    //si da per scontato che il dispositvo sia acceso
    return (energyUse*(current_time-start_time));
}

