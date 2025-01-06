//DAVIDE MEROTTO

#ifndef DEV_H
#define DEV_H

#include <string>
#include "Time.h"

//CLASSE BASE 
class Device{

    //funzioni membro
    public: 
        Device() = delete;    //non permetto la creazione di oggetti di tipo Device
        void stopDevice();
        void startDevice();
        std::string getName();
        const int getID();
        bool getStatus();
        Time getEndTime();
        double getPowerUseByTime(Time& current_time);        

    //Data membri
    protected:
        Time start_time;
        Time end_time;
        std::string name;
        const int id;
        double powerUse;  
        bool status;
};

#endif DEV_H