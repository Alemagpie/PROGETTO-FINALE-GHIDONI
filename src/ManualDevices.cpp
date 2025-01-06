//DAVIDE MEROTTO

#include "../include/ManualDevice.h"
#include "../include/Time.h"

//Implementazione classe ManualDevices
ManualDevice:: ManualDevice(std::string nome, const int ID, double en)
    : Device(nome, ID, en) {
}

void ManualDevice:: setTimer(Time& start_t){

    start_time = start_t;
}

void ManualDevice::resetTimer() {

    //reset timer
    start_time.setTime(0,0);
}