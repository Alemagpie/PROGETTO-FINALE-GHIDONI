//DAVIDE MEROTTO

#include "../include/ManualDevice.h"

#include <string>

#include "../include/CustomTime.h"

//Implementazione classe ManualDevices
ManualDevice:: ManualDevice(std::string nome, const int ID, double power)
    :  Device(nome, ID, power) {
    
    start_time_.SetTime(0,0);
    end_time_.SetTime(23,59);
}

void ManualDevice:: SetTimer(const CustomTime& start_t, CustomTime end_t){
    start_time_ = start_t;
    end_time_ = end_t;
}