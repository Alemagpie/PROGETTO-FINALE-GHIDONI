//DAVIDE MEROTTO

#include "../include/AutomaticDevice.h"

#include <string>

#include "../include/CustomTime.h"

AutomaticDevice::AutomaticDevice(std::string nome, const int ID, double power, CustomTime duration) 
: Device(nome, ID, power) {
    time_duration_ = duration;
    start_time_.SetTime(0,0);
    end_time_.SetTime(0,0);
}

void AutomaticDevice:: SetTimer(CustomTime& start_t , CustomTime end_t){
    //end_t non considerata -> tempo finale dipende solo da quello iniziale e dalla durata di accensione
    start_time_ = start_t;
    end_time_ = start_t + time_duration_;
}