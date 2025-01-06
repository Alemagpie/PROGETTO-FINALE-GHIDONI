//DAVIDE MEROTTO

#include "../include/ManualDevice.h"
#include "../include/Time.h"

//Implementazione classe ManualDevices
ManualDevice:: ManualDevice(std::string nome, const int ID, double power)
    :  name{nome}, id{ID}, powerUse{power}, status{false}{
    
    start_time.setTime(0,0);
    end_time.setTime(0,0);
}

void ManualDevice:: setTimer(Time& start_t, Time& end_t){

    start_time = start_t;
    end_time = end_t;
}
