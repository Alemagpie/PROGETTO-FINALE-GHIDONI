//DAVIDE MEROTTO

#ifndef AUTO_DEV_H
#define AUTO_DEV_H

#include "Device.h"
#include <string>

//SOTTOCLASSE
class AutomaticDevice : public Device{

    //Costruttore e funzioni membro
    public:
        AutomaticDevice(std::string nome, const int ID, double power, CustomTime duration);
        void setTimer(CustomTime& start_t , CustomTime end_t) override;
        virtual void removeTimer() override;
        void updateEndTime(CustomTime newEndTime = CustomTime(23,59)) override;
        void updateStartTime(CustomTime newStartTime) override;
        char getType() override;

    private:
        CustomTime time_duration;
};

#endif