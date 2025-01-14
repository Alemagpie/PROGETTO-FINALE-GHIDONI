//ALESSIO FAGNANI (DESIGN DELL'INTERFACCIA)

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
        //Costruttore parametrizzato
        DeviceManager(OutputManager& outPut);
        
        //Aggiungi entry a multimappa dei dispositivi attivi con value "dev"
        void AddDevice(Device* const dev); 
        //Aggiungi il dispositivo nel vettore che tiene traccia di tutti i device
        void AddDeviceToList( Device& new_dev);
        //Aggiunti entry a multimappa dei device con accensione ritardata con chiave "start" e valore "<end, dev>"
        void AddDeviceAsync(Device* dev, CustomTime start, CustomTime end); 
        //Sposta il puntatore al dispositivo dalla multimappa dei device "pending" nella multimappa degli attivi
        void MoveDevice(std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it);
        //Rimuove entry dalla multimappa dei device attivi
        void RemoveDevice(std::multimap<CustomTime, Device*>::iterator it); 
        //Controlla che il dispositivo, se aggiunto, non faccia sforare il limite di consumo (true se è nel limite, altrimenti false)
        bool CheckPowerConsumption(Device* const d) const;
        //Cambia l'orario di sistema
        void SetTime(CustomTime new_time);  
        //valuta l'input  
        void ParseInput(std::string command);
        //Controlla se si è a fine giornata   
        inline bool GetFineGiornata() const { return fine_giornata_; }
        //Resetta l'orario a 00:00
        void ResetTime();
        //Rimuove le entry della multimappa dei device ad accensione programmata
        void ResetTimers();

    private:
        //Multimappa dei dispositivi attivi
        std::multimap<CustomTime, Device*> active_devices_; 
        //Multimappa dei dispositivi in attesa dell'attivazione (hanno come chiave il tempo di inizio, come valore il tempo di fine e il ptr al device)
        std::multimap<CustomTime, std::pair<CustomTime, Device*>> async_devices_;  
        //Vettore di ptr ai Device
        std::vector<Device*> device_list_; 
        //Vettore che memorizza l'ordine di accensione dei device
        std::vector<Device*> device_insert_order_;

        //Output manager
        OutputManager& out_;
        //Massima potenza consentita (in kW)
        const double kMaxPower = 3.5;
        //Potenza corrente
        double power_use_;
        //true se è fine giornata, false altrimenti
        bool fine_giornata_;
        //Orario di sistema
        CustomTime current_time_;  
        //lunghezza del vettore di Device 
        int device_count_;    

        //
        void PrintInfoAll(std::string_view rem) const;
        //
        void PrintInfoAsync(std::string_view rem) const;

};
//DA SPOSTARE
void SentenceIntoWords( std::vector<std::string>& ret, std::string sentence);
//Trasforma una stringa in CustomTime
CustomTime StringIntoCTime(std::string orario);
#endif