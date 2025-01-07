//DAVIDE MEROTTO

#include "../include/AutomaticDevice.h"
#include "../include/Time.h"

AutomaticDevice::AutomaticDevice(std::string nome, const int ID, double power, Time duration) 
: Device(nome, ID, power) {
    time_duration = duration;
    start_time.setTime(0,0);
    end_time.setTime(0,0);
}

void AutomaticDevice:: setTimer(Time& start_t){
    start_time=start_t;
    end_time=start_t + time_duration;
}
