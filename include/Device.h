//DAVIDE MEROTTO

#ifndef DEV_H
#define DEV_H

#include <string>
#include "CustomTime.h"

//CLASSE BASE 
class Device{
    //funzioni membro
    public: 
        //evito slicing
        Device(const Device&) = delete;
        Device& operator= (const Device&) = delete;

        void stopDevice();
        void startDevice();
        std::string getName() const;
        int getID() const;
        bool getStatus() const;
        CustomTime getStartTime() const;
        CustomTime getEndTime() const;
        const double getCurrentPowerConsumption();
        const double getPowerUsed();
        void setStatus(bool value);
        void updatePowerUsed(CustomTime& currTime);
        void reset();

        //funzioni virtuali, rendono la classe astratta-> funzioni da sovrascrivere nelle sottoclassi
        virtual void updateStartTime(CustomTime newStartTime) =0;
        virtual void updateEndTime(CustomTime newEndTime = CustomTime(23,59)) =0;
        virtual void setTimer(CustomTime& start_t, CustomTime end_t ) = 0;     
        virtual void removeTimer() =0;
        
    //Data membri
    protected:
        CustomTime start_time;
        CustomTime end_time;
        std::string name;
        const int id;
        double powerConsumption;
        double powerUsed;  
        bool status;

        //non permetto la creazione di oggetti di tipo Device (la classe è comunque astratta )
        Device(std::string nome, const int ID, double power);
};

bool operator==(const Device& a, const Device& b);

std::ostream& operator<<(std::ostream& os, Device& dev);

#endif