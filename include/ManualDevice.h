//DAVIDE MEROTTO

#ifndef PROGETTOFINALE_SRC_MANUALDEVICE_H
#define PROGETTOFINALE_SRC_MANUALDEVICE_H

#include "Device.h"

#include <string>

#include "CustomTime.h"

//Sottoclasse -> dispositivi di tipo manuali
class ManualDevice : public Device{

    //Costruttore e funzioni membro
    public:
        ManualDevice(std::string nome, const int ID, double power);
        
        void SetTimer(const CustomTime& start_t, CustomTime end_t) override;
        //La funzione per i dispositivi manuali comporta l'aggiornamento dell'endtime alle ore 23:59 
        inline virtual void RemoveTimer() override  {UpdateEndTime();}
        inline void UpdateStartTime(CustomTime newStartTime) override   {start_time_ = newStartTime;}
        inline void UpdateEndTime(CustomTime newEndTime) override   {end_time_ = newEndTime;}
        inline void UpdateEndTime () override {end_time_ = CustomTime(23,59);}
        inline char GetType() const override {return 'M';}
};

#endif