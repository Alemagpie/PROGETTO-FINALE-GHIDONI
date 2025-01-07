//DAVIDE MEROTTO

#ifndef DEV_H
#define DEV_H

#include <string>
#include "Time.h"

//CLASSE BASE 
class Device{

    protected: 
        //non permetto la creazione di oggetti di tipo Device
        Device(std::string nome, const int ID, double power);

    //funzioni membro
    public: 
        //evito slicing
        Device(const Device&) = delete;
        Device& operator= (const Device&) = delete;
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