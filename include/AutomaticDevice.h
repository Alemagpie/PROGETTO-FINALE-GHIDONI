//DAVIDE MEROTTO

#ifndef PROGETTOFINALE_SRC_AUTOMATICDEVICE_H
#define PROGETTOFINALE_SRC_AUTOMATICDEVICE_H

#include "Device.h"

#include <string>

#include "CustomTime.h"

//SOTTOCLASSE
class AutomaticDevice : public Device{

    //Costruttore e funzioni membro
    public:
        AutomaticDevice(std::string nome, const int ID, double power, CustomTime duration);

        void setTimer(CustomTime& start_t , CustomTime end_t) override;
        inline virtual void removeTimer() override {return;}
        inline void updateEndTime(CustomTime newEndTime = CustomTime(23,59)) override {end_time  = start_time + time_duration;}
        inline void updateStartTime(CustomTime newStartTime) override {start_time = newStartTime;}
        inline char getType() override {return 'A';}

    private:
        CustomTime time_duration;
};

#endif