//DAVIDE MEROTTO

#include "OutputManager.h"

template <typename T>
OutputManager& OutputManager::operator << (const T &valueToPrint){
    output1 << valueToPrint;
    output2 << valueToPrint;
    //libera il buffer e scrivi su file zz
    output2.flush();
    return *this;
}

template <typename T>
OutputManager& OutputManager::operator << (T& valueToPrint){
    output1 << valueToPrint;
    output2 << valueToPrint;
    //libera il buffer e scrivi sul file
    output2.flush();
    return *this;
}