//DAVIDE MEROTTO

#ifndef MAN_DEV_H
#define MAN_DEV_H

#include "Device.h"
#include <string>

//SOTTOCLASSE
class ManualDevice : public Device{

    //Costruttore e funzioni membro
    public:
        ManualDevice(std::string nome, const int ID, double power);
        void setTimer(Time& start_t, Time& end_t);
};

#endif