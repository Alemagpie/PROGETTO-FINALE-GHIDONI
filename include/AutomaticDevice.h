// DAVIDE MEROTTO

#ifndef AUTO_DEV_H
#define AUTO_DEV_H

#include "Device.h"
#include <string>

//SOTTOCLASSE
class AutomaticDevice : public Device{

    //Costruttore e funzioni membro
    public:
        AutomaticDevice(std::string nome, const int ID, double en);
        void setTimer(Time& start_time, Time& stop_time);
        void resetTimer();
    //Data membri
    private:
        Time stop_time;
    
};

#endif