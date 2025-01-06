//DAVIDE MEROTTO

#include "../include/ManualDevices.h"
#include "../include/Time.h"

//Implementazione classe ManualDevices
ManualDevices:: ManualDevices(String name, const int id, double wattProduction, bool status)
    : this.name{name}, this.id{id}, this.wattProduction{wattProduction}, this.status{status}, this.start_time{0,0}{
}

void ManualDevices:: setTimer(Time& start_time){

    this.start_time=start_time;
}

void ManualDevices:: resetTimer() override{

    //reset timer
    start_time.setTime(0,0);
}