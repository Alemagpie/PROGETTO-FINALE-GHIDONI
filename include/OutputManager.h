//DAVIDE MEROTTO

#ifndef PROGETTOFINALE_SRC_OUTPUTMANAGER_H
#define PROGETTOFINALE_SRC_OUTPUTMANAGER_H

#include <iostream>
#include <ostream>

//classe per la gestione degli output : cout e File
class OutputManager{

    //costruttore e sovrascrittura operatore << (caso const e non const)
    public: 
        inline OutputManager(std:: ostream &o1, std:: ostream &o2)
            : output1{o1}, output2{o2}{};

        template <typename T>
        OutputManager& operator << (const T &valueToPrint);

        template <typename T>
        OutputManager& operator << (T& valueToPrint);

    private:
        std::ostream &output1 , &output2;

};

#include "OutputManager.hpp"

#endif