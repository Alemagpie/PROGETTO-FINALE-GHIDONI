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

        void AddDevice(Device* dev);  //aggiungi a multimappa degli attivi
        void AddDeviceToList(Device& new_dev);
        void AddDeviceAsync(Device* dev, CustomTime start, CustomTime end); //aggiungi a multimappa dei "pending"
        void MoveDevice(std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it);
        void RemoveDevice(std::multimap<CustomTime, Device*>::iterator it); //rimuovi dispositivo in base al nome, fa il return del dispositivo
        bool CheckPowerConsumption(Device* d);
        double CheckPowerConsumptionGeneral();
        void SetTime(CustomTime new_time);    //cambia orario
        void ParseInput(std::string command);   //valuta input
        inline bool GetFineGiornata() { return fine_giornata_; }
        void ResetTime();
        void ResetTimers();

    private:
        std::multimap<CustomTime, Device*> active_devices_; //multimappa dei dispositivi attivi
        std::multimap<CustomTime, std::pair<CustomTime, Device*>> async_devices_;  //multimappa dei dispositivi in attesa dell'attivazione (hanno come chiave il tempo di inizio, come valore il tempo di fine e il ptr al device)
        std::vector<Device*> device_list_; //vettore di ptr ai Device
        std::vector<Device*> device_insert_order_;
    
        OutputManager& out_;
        const double kMaxPower = 3.5; //in kW
        double power_use_;
        bool fine_giornata_;
        CustomTime current_time_;   
        int device_count_;    //lunghezza del vettore di Device
    
        void PrintInfoAll(std::string_view rem);
        void PrintInfoAsync(std::string_view rem);

};

void SentenceIntoWords(std::vector<std::string>& ret, std::string sentence);
CustomTime StringIntoCTime(std::string orario);


#endif