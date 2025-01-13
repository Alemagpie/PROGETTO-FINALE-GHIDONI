#ifndef OUT_H
#define OUT_H

#include <iostream>
#include <ostream>

//classe per la gestione degli output : cout e File
class OutputManager{

    private:
        std::ostream &output1 , &output2;

    //costruttore e sovrascrittura operatore << (caso const e non const)
    public: 
        OutputManager(std:: ostream &o1, std:: ostream &o2)
            : output1{o1}, output2{o2}{};

        template <typename T>
        OutputManager& operator << (const T &valueToPrint){
            output1 << valueToPrint;
            output2 << valueToPrint;
            //libera il buffer e scrivi su file zz
            output2.flush();
            return *this;
        }

        template <typename T>
        OutputManager& operator << (T& valueToPrint){
            output1 << valueToPrint;
            output2 << valueToPrint;
            //libera il buffer e scrivi sul file
            output2.flush();
            return *this;
        }
};

#endif