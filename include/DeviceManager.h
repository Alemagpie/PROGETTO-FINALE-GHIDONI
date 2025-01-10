#ifndef DEV_MANAGER_H
#define DEV_MANAGER_H

#include <iterator>
#include <utility> //per usare std::pair nella multimappa
#include <map>
#include "ManualDevice.h"
#include "AutomaticDevice.h"
#include "CustomTime.h"
#include <iostream>
#include <vector>
#include <string>

//albero dei dispositivi
class DeviceManager {
    public:
    DeviceManager();
    void addDevice(Device* dev);  //aggiungi a multimappa degli attivi
    void addDeviceToList(Device& newDev);
    void setDeviceStartTime();
    void addDeviceAsync(Device& d); //aggiungi a multimappa dei "pending"
    Device getDevice(CustomTime t);   //trova dispositivo in base a tempo
    Device* removeDevice(std::multimap<CustomTime, Device*>::iterator it); //rimuovi dispositivo in base al nome, fa il return del dispositivo
    Device* removeDevicesByTime(CustomTime t); //rimuovi uno o più dispositivi con orario uguale o passato rispetto a quello corrente, fa il return del primo dispositivo

    void checkOnHourChange();   //controlla multimappa async e nel caso aggiunge, controlla multimappa attivi e nel caso rimuove

    void setTime(CustomTime newTime);    //cambia orario

    void parseInput(std::string command);   //valuta input

    private:
    std::multimap<CustomTime, Device*> activeDevices; //multimappa dei dispositivi attivi
    std::multimap<CustomTime, std::pair<CustomTime, Device*>> asyncDevices;  //multimappa dei dispositivi in attesa dell'attivazione (hanno come chiave il tempo di inizio, come valore il tempo di fine e il ptr al device)
    std::vector<Device*> deviceList; //vettore di ptr ai Device
    CustomTime currentTime;   
    CustomTime currentDeviceEndTime;

    int deviceCount;    //lunghezza del vettore di Device
    Device* currentDevice;  //puntatore al Device su cui si sta lavorando
    
    std::multimap<CustomTime, Device*>::iterator findDevice(Device& d);
    std::multimap<CustomTime, Device*>::iterator findDeviceByID(int ID);
    std::multimap<CustomTime, Device*>::iterator findDeviceByNameActive(std::string& s);
    std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator findDeviceByNameAsync(std::string& s);
    std::vector<Device*>::iterator findDeviceByNameAll(std::string& s);
    void print_infoAll(std::string_view rem);

};

void SentenceIntoWords(std::vector<std::string>& ret, std::string sentence);

#endif