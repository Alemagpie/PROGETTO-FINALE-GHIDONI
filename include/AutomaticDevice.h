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

        void SetTimer(const CustomTime& start_t , CustomTime end_t) override;
        inline virtual void RemoveTimer() override {return;}
        inline void UpdateEndTime(CustomTime newEndTime) override {end_time_  = start_time_ + time_duration_;}
        inline void UpdateEndTime() override {UpdateEndTime(CustomTime(23,59));}
        inline void UpdateStartTime(CustomTime newStartTime) override {start_time_ = newStartTime;}
        inline char GetType() const override {return 'A';}

    private:
        CustomTime time_duration_;
};

#endif