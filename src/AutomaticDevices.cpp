//DAVIDE MEROTTO

#include "../include/AutomaticDevices.h"
#include "../include/Time.h"

AutomaticDevices:: AutomaticDevices(String name, const int id, double wattProduction, bool status)
    : this.name{name}, this.id{id}, this.wattProduction{wattProduction}, this.status{status}, this.start_time{0,0}, this.stop_time{0,0}{
}

void AutomaticDevices:: setTimer(Time& start_time, Time& stop_time){

    this.start_time=start_time;
    this.stop_time=stop_time;
}

void AutomaticDevices:: resetTimer() override{

    //reset timer
    start_time.setTime(0,0);
    stop_time.setTime(0,0);
}