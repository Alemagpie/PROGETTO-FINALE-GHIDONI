//DAVIDE MEROTTO

#include "../include/Device.h"

Device::Device(std::string nome, const int ID, double power) 
: name{nome}, id{ID}, powerUse{power}, status{false} {}

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

const double Device:: getPowerUseByTime(Time& current_time){
    //si da per scontato che il dispositvo sia acceso
    return powerUse * (current_time - start_time);
}

bool operator==(const Device& a, const Device& b) {
    return (a.getName() == b.getName() && a.getID() == b.getID());
}