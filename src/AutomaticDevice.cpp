//DAVIDE MEROTTO

#include "../include/AutomaticDevice.h"
#include "../include/CustomTime.h"

AutomaticDevice::AutomaticDevice(std::string nome, const int ID, double power, CustomTime duration) 
: Device(nome, ID, power) {
    time_duration = duration;
    start_time.setTime(0,0);
    end_time.setTime(0,0);
}

void AutomaticDevice:: setTimer(CustomTime& start_t , CustomTime end_t){
    //end_t non considerata -> tempo finale dipende solo da quello iniziale e dalla durata di accensione
    start_time = start_t;
    end_time = start_t + time_duration;
}

void AutomaticDevice::removeTimer(){
    return;
}
