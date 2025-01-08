//DAVIDE MEROTTO

#ifndef DEV_H
#define DEV_H

#include <string>
#include "Time.h"

//CLASSE BASE 
class Device{

    protected: 
        //non permetto la creazione di oggetti di tipo Device (la classe è comunque astratta )
        Device(std::string nome, const int ID, double power);

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
        Time getEndTime() const;
        const double getPowerUseByTime(Time& current_time); 
        virtual void setTimer(Time& start_t, Time& end_t) = 0;  //rende la classe astratta    

    //Data membri
    protected:
        Time start_time;
        Time end_time;
        std::string name;
        const int id;
        double powerUse;  
        bool status;
};

bool operator==(const Device& a, const Device& b);

#endif