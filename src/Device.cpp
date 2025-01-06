//DAVIDE MEROTTO

#include "../include/Device.h"

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

Time Device:: getEndTime() {
    return end_time;
}

double Device:: getPowerUseByTime(Time& current_time){
    
    //si da per scontato che il dispositvo sia acceso
    return (powerUse*(current_time-start_time));
}

