//DAVIDE MEROTTO

#ifndef DEV_H
#define DEV_H

#include <string>
#include "Time.h"

//CLASSE BASE 
class Device{

    //funzioni membro
    public: 
        Device(std::string nome, const int ID, double en);
        void stopDevice();
        void startDevice();
        std::string getName();
        const int getID();
        bool getStatus();
        Time getEndTime();
        double getEnergyUseByTime(Time& current_time);        

    //Data membri
    protected:
        Time start_time;
        Time end_time;
        std::string name;
        const int id;
        double energyUse;  //può essere sia negativo che positivo, meglio non chiamarla "produzione"
        bool status;

        virtual void resetTimer()=0;    //funzione virtuale pura -> da sovrascrivere
};

#endif DEV_H