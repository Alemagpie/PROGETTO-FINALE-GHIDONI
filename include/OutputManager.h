//DAVIDE MEROTTO

#ifndef PROGETTOFINALE_SRC_OUTPUTMANAGER_H
#define PROGETTOFINALE_SRC_OUTPUTMANAGER_H

#include <iostream>
#include <ostream>
#include <fstream>

//Classe per la gestione degli output : cout e file
class OutputManager{

    //Costruttore e sovrascrittura operatore << (casi argomento const e non const)
    public: 
        inline OutputManager(std:: ostream &o1, std:: ofstream &o2)
            : output1_{o1}, output2_{o2}{};

        template <typename T>
        OutputManager& operator << (const T &valueToPrint);

        template <typename T>
        OutputManager& operator << (T& valueToPrint);

    private:
        std::ostream &output1_ ;
        std::ofstream &output2_;

};

#include "OutputManager.hpp"

#endif