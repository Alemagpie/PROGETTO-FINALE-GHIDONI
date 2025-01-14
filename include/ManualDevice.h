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
        
        void SetTimer(CustomTime& start_t, CustomTime end_t) override;
        inline virtual void RemoveTimer() override  {end_time_.SetTime(23,59);}
        inline void UpdateStartTime(CustomTime newStartTime) override   {start_time_ = newStartTime;}
        inline void UpdateEndTime(CustomTime newEndTime) override   {end_time_ = newEndTime;}
        inline void UpdateEndTime () override {end_time_ = CustomTime(23,59);}
        inline char GetType() override {return 'M';}
};

#endif