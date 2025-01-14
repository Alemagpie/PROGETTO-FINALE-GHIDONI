//DAVIDE MEROTTO

#include "OutputManager.h"

template <typename T>
OutputManager& OutputManager::operator << (const T &valueToPrint){
    //Stampo sul terminale e sul file
    output1_ << valueToPrint;
    output2_ << valueToPrint;
    //Libera il buffer associato al flusso del file per scrivere subito sul file
    output2_.flush();
    return *this;
}

template <typename T>
OutputManager& OutputManager::operator << (T& valueToPrint){
    //Stampo sul terminale e sul file
    output1_ << valueToPrint;
    output2_ << valueToPrint;
    //Libera il buffer associato al flusso del file per scrivere subito sul file
    output2_.flush();
    return *this;
}