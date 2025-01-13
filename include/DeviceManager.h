//ALESSIO FAGNANI

#ifndef PROGETTOFINALE_SRC_DEVICEMANAGER_H
#define PROGETTOFINALE_SRC_DEVICEMANAGER_H

#include <iterator>
#include <utility> //per std::pair
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>

#include "ManualDevice.h"
#include "AutomaticDevice.h"
#include "CustomTime.h"
#include "OutputManager.h"
#include "Utility.h"

//albero dei dispositivi
class DeviceManager {
    public:
        DeviceManager(OutputManager& outPut);

        void addDevice(Device* dev);  //aggiungi a multimappa degli attivi
        void addDeviceToList(Device& newDev);
        void addDeviceAsync(Device* dev, CustomTime Start, CustomTime End); //aggiungi a multimappa dei "pending"
        void moveDevice(std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it);
        void removeDevice(std::multimap<CustomTime, Device*>::iterator it); //rimuovi dispositivo in base al nome, fa il return del dispositivo
        bool checkPowerConsumption(Device* d);
        double checkPowerConsumptionGeneral();
        void setTime(CustomTime newTime);    //cambia orario
        void parseInput(std::string command);   //valuta input
        inline bool getFineGiornata() { return fineGiornata; }
        void resetTime();
        void resetTimers();

    private:
        std::multimap<CustomTime, Device*> activeDevices; //multimappa dei dispositivi attivi
        std::multimap<CustomTime, std::pair<CustomTime, Device*>> asyncDevices;  //multimappa dei dispositivi in attesa dell'attivazione (hanno come chiave il tempo di inizio, come valore il tempo di fine e il ptr al device)
        std::vector<Device*> deviceList; //vettore di ptr ai Device
        std::vector<Device*> deviceInsertOrder;
    
        OutputManager& out;
        const double maxPower = 3.5; //in kW
        double powerUse;
        bool fineGiornata;
        CustomTime currentTime;   
        int deviceCount;    //lunghezza del vettore di Device
    
        void print_infoAll(std::string_view rem);
        void print_infoAsync(std::string_view rem);

};

void SentenceIntoWords(std::vector<std::string>& ret, std::string sentence);
CustomTime StringIntoCTime(std::string orario);


#endif