//DAVIDE MEROTTO

#ifndef PROGETTOFINALE_SRC_MANUALDEVICE_H
#define PROGETTOFINALE_SRC_MANUALDEVICE_H

#include "Device.h"

#include <string>

#include "../include/CustomTime.h"

//SOTTOCLASSE
class ManualDevice : public Device{

    //Costruttore e funzioni membro
    public:
        ManualDevice(std::string nome, const int ID, double power);
        void setTimer(CustomTime& start_t, CustomTime end_t) override;
        inline virtual void removeTimer() override  {end_time.setTime(23,59);}
        inline void updateStartTime(CustomTime newStartTime) override   {start_time = newStartTime;}
        inline void updateEndTime(CustomTime newEndTime = CustomTime(23,59)) override   {end_time = newEndTime;}
        inline char getType() override {return 'M';}
};

#endif