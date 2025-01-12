#ifndef OUT_H
#define OUT_H

#include <iostream>
#include <ostream>

//classe per la gestione degli output : cout e File
class OutputManager{

    private:
        std::ostream &output1 , &output2;

    public: 
        OutputManager(std:: ostream &o1, std:: ostream &o2)
            : output1{o1}, output2{o2}{}

        //sovrascrittura operatore <<
        template <typename T>
        OutputManager& operator << (const T &valueToPrint){
            output1 << valueToPrint;
            output2 << valueToPrint;
            return *this;
        }
};

#endif