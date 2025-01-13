//DAVIDE MEROTTO

#include "../include/ManualDevice.h"

#include <string>

#include "../include/CustomTime.h"

//Implementazione classe ManualDevices
ManualDevice:: ManualDevice(std::string nome, const int ID, double power)
    :  Device(nome, ID, power) {
    
    start_time.setTime(0,0);
    end_time.setTime(23,59);
}

void ManualDevice:: setTimer(CustomTime& start_t, CustomTime end_t){
    start_time = start_t;
    end_time = end_t;
}