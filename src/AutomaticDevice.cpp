//DAVIDE MEROTTO

#include "../include/AutomaticDevice.h"
#include "../include/Time.h"

AutomaticDevice::AutomaticDevice(std::string nome, const int ID, double en) 
: Device(nome, ID, en) {}

void AutomaticDevice:: setTimer(Time& start_t, Time& stop_t){

    start_time=start_t;
    stop_time=stop_t;
}

void AutomaticDevice:: resetTimer() {

    //reset timer
    start_time.setTime(0,0);
    stop_time.setTime(0,0);
}