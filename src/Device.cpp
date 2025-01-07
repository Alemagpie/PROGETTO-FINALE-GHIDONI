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

std::string Device:: getName(){ 
    return name;
}

const int Device:: getID(){ 
    return id;
}

bool Device:: getStatus(){ 
    return status;
}

double Device:: getPowerUseByTime(Time& current_time){
    
    //si da per scontato che il dispositvo sia acceso
    return powerUse * (current_time - start_time);
}

