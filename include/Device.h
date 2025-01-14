//DAVIDE MEROTTO

#ifndef PROGETTOFINALE_SRC_DEVICE_H
#define PROGETTOFINALE_SRC_DEVICE_H

#include <string>

#include "CustomTime.h"

//CLASSE BASE 
class Device{
    //funzioni membro
    public: 
        //evito slicing
        Device(const Device&) = delete;
        Device& operator= (const Device&) = delete;

        inline void StopDevice()    {status_=false;}
        inline void StartDevice()   {status_=true;}
        inline std::string GetName() const  {return name_;}
        inline int GetID() const    {return id_;}
        inline bool GetStatus() const   {return status_;}
        inline CustomTime GetStartTime() const  {return start_time_;}
        inline CustomTime GetEndTime() const    {return end_time_;}
        inline const double GetCurrentPowerConsumption()    {return power_consumption_;}
        inline const double GetPowerUsed()  {return power_used_;}
        void UpdatePowerUsed(CustomTime& currentTime);
        void Reset();

        //funzioni virtuali, rendono la classe astratta-> funzioni da sovrascrivere nelle sottoclassi
        virtual void UpdateStartTime(CustomTime newStartTime) =0;
        virtual void UpdateEndTime(CustomTime newEndTime ) =0;
        virtual void UpdateEndTime() =0;
        virtual void SetTimer(CustomTime& start_t, CustomTime end_t ) = 0;     
        virtual void RemoveTimer() =0;
        virtual char GetType() =0;
        
    //Data membri
    protected:

        //non permetto la creazione di oggetti di tipo Device (la classe è comunque astratta )
        Device(std::string nome, const int ID, double power);

        CustomTime start_time_;
        CustomTime end_time_;
        std::string name_;
        const int id_;
        double power_consumption_;
        double power_used_;  
        bool status_;

        
};

inline bool operator==(const Device& a, const Device& b)    {return (a.GetName() == b.GetName() && a.GetID() == b.GetID());}

std::ostream& operator<<(std::ostream& os, Device& dev);

#endif