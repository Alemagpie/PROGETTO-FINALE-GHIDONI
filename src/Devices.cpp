//DAVIDE MEROTTO

#include "../include/Devices.h"

//Implementazione classe Devices
void Devices:: stopDevice(){ 
    status=false;
}

void Devices:: startDevice(){                
    status=true;
}

String Devices:: getName(){ 
    return name;
}

const int Devices:: getID(){ 
    return id;
}

bool Devices:: getStatus(){ 
    return status;
}

double Devices:: getWattHourProduction(Time& current_time){
    
    //si da per scontato che il dispositvo sia acceso
    return (wattProduction*(current_time-start_time));
}

