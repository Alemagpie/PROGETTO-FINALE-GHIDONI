//DAVIDE MEROTTO

#include "OutputManager.h"

template <typename T>
OutputManager& OutputManager::operator << (const T &valueToPrint){
    output1_ << valueToPrint;
    output2_ << valueToPrint;
    //libera il buffer e scrivi su file zz
    output2_.flush();
    return *this;
}

template <typename T>
OutputManager& OutputManager::operator << (T& valueToPrint){
    output1_ << valueToPrint;
    output2_ << valueToPrint;
    //libera il buffer e scrivi sul file
    output2_.flush();
    return *this;
}