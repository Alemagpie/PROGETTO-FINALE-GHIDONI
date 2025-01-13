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

        inline void stopDevice()    {status=false;}
        inline void startDevice()   {status=true;}
        inline std::string getName() const  {return name;}
        inline int getID() const    {return id;}
        inline bool getStatus() const   {return status;}
        inline CustomTime getStartTime() const  {return start_time;}
        inline CustomTime getEndTime() const    {return end_time;}
        inline const double getCurrentPowerConsumption()    {return powerConsumption;}
        inline const double getPowerUsed()  {return powerUsed;}
        void updatePowerUsed(CustomTime& currTime);
        void reset();

        //funzioni virtuali, rendono la classe astratta-> funzioni da sovrascrivere nelle sottoclassi
        virtual void updateStartTime(CustomTime newStartTime) =0;
        virtual void updateEndTime(CustomTime newEndTime = CustomTime(23,59)) =0;
        virtual void setTimer(CustomTime& start_t, CustomTime end_t ) = 0;     
        virtual void removeTimer() =0;
        virtual char getType() =0;
        
    //Data membri
    protected:

        //non permetto la creazione di oggetti di tipo Device (la classe è comunque astratta )
        Device(std::string nome, const int ID, double power);

        CustomTime start_time;
        CustomTime end_time;
        std::string name;
        const int id;
        double powerConsumption;
        double powerUsed;  
        bool status;

        
};

inline bool operator==(const Device& a, const Device& b)    {return (a.getName() == b.getName() && a.getID() == b.getID());}

std::ostream& operator<<(std::ostream& os, Device& dev);

#endif