#ifndef DEV_MANAGER_H
#define DEV_MANAGER_H

#include <iterator>
#include <utility> //per usare std::pair nella multimappa
#include <map>
#include "Device.h"
#include "Time.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

//albero dei dispositivi
class DeviceManager {
    public:
    DeviceManager();

    private:
    std::multimap<Time, Device*> activeDevices; //multimappa dei dispositivi attivi
    std::multimap<Time, std::pair<Time, Device*>> asyncDevices;  //multimappa dei dispositivi in attesa dell'attivazione (hanno come chiave il tempo di inizio, come valore il tempo di fine e il ptr al device)
    std::vector<Device> deviceList; //vettore di Device ordinato per (ID/nome)
    Time currentTime;   
    Time currentDeviceEndTime;

    int deviceCount;    //lunghezza del vettore di Device
    Device* currentDevice;  //puntatore al Device su cui si sta lavorando

    void addDevice();  //aggiungi a multimappa degli attivi
    void addDeviceAsync(Device& d); //aggiungi a multimappa dei "pending"
    Device getDevice(Time t);   //trova dispositivo in base a tempo
    Device* removeDevice(std::multimap<Time, Device*>::iterator it); //rimuovi dispositivo in base al nome, fa il return del dispositivo
    Device* removeDevicesByTime(Time t); //rimuovi uno o più dispositivi con orario uguale o passato rispetto a quello corrente, fa il return del primo dispositivo

    void checkOnHourChange();   //controlla multimappa async e nel caso aggiunge, controlla multimappa attivi e nel caso rimuove

    void setTime(Time& newTime);    //cambia orario

    void parseInput(std::string command);   //valuta input
    
    std::multimap<Time, Device*>::iterator findDevice(Device& d);
    std::multimap<Time, Device*>::iterator findDeviceByID(int ID);
    std::multimap<Time, Device*>::iterator findDeviceByName(std::string& s);

};



#endif