//DAVIDE MEROTTO

#ifndef AUTO_DEV_H
#define AUTO_DEV_H

#include "Device.h"
#include <string>

//SOTTOCLASSE
class AutomaticDevice : public Device{

    //Costruttore e funzioni membro
    public:
        AutomaticDevice(std::string nome, const int ID, double power, Time duration);
        void setTimer(Time& start_t , Time& end_t) override;

    private:
        Time time_duration;
};

#endif