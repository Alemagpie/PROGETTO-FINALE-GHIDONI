//DAVIDE MEROTTO

#ifndef DEV_H
#define DEV_H

#include <string>
#include "CustomTime.h"

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
        CustomTime getEndTime() const;
        const double getPowerUsed(); 
        virtual void setTimer(CustomTime& start_t, CustomTime& end_t) = 0;  //rende la classe astratta    

    //Data membri
    protected:
        CustomTime start_time;
        CustomTime end_time;
        std::string name;
        const int id;
        double Consumption;
        double powerUsed;  
        bool status;
};

bool operator==(const Device& a, const Device& b);

std::ostream& operator<<(std::ostream& os, Device& dev);

#endif