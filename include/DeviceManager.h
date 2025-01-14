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

//Classe che permette
class DeviceManager {
    public:
        //Costruttore parametrizzato
        DeviceManager(OutputManager& outPut);

        //Inserisce un puntatore ad un device nella multimappa che contiene i device attivi,
        //con chiave il suo orario di spegnimento
        void AddDevice(Device* const dev);
        //Inserisce un puntatore ad un device nella lista che contiene tutti i device da gestire
        void AddDeviceToList( Device& new_dev);
        //Inserisce un std::pair<CustomTime, Device*> nella multimappa che contiene i device 
        //con accensione posticipata, con chiave il suo orario di inizio
        void AddDeviceAsync(Device* dev, CustomTime start, CustomTime end);
        //Permette di rimuovere un device dalla multimappa dei device con accensione posticipata,
        //e di inserirlo nella multimappa dei device accesi con il giusto orario di spegnimento
        void MoveDevice(std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it);
        //Rimuove un device dalla multimappa dei device accesi, dato che e' giunto al tempo
        //di spegnimento
        void RemoveDevice(std::multimap<CustomTime, Device*>::iterator it); 
        //Controlla se l'aggiunta di un eventuale device faccia superare il limite di potenza
        //massima del sistema.
        //Restituisce true se non la supera, false se la supera.
        bool CheckPowerConsumption(Device* const d) const;
        //Funzione che simula il passare del tempo. Controlla se tra il tempo attuale e 
        //il nuovo tempo alcuni dispositivi debbano accendersi o spegnersi, e in tal
        //caso li accende/spegne.
        void SetTime(CustomTime new_time);    
        //Fa in modo che il comando inserito sotto forma di stringa corrisponda alla
        //funzione richiesta
        void ParseInput(std::string command); 
        inline bool GetFineGiornata() const { return fine_giornata_; }
        //Imposta l'orario della gioranta alle 00:00 e spegne tutti i dispositivi,
        //mantenendo comunque i timer già attivi
        void ResetTime();
        //Resetta tutti i timer dei dispositivi
        void ResetTimer(std::vector<Device*>::iterator iterAll);

    private:
        //Multimappa che contiene std::pair con chiave l'orario di spegnimento,
        //e con value il puntatore al suo dispositivo acceso relativo
        std::multimap<CustomTime, Device*> active_devices_;
        //Multimappa che contiene std::pair con chiave l'orario di accensione programmata,
        //e con value la std::pair composta dall'orario di spegnimento desiderato (per i 
        //dispositivi manuali, altrimenti quello di base per gli automatici) e dal puntatore 
        //al device che si intende accendere 
        std::multimap<CustomTime, std::pair<CustomTime, Device*>> async_devices_; 
        //Vettore composto da tutti i puntatori ai dispositivi gestiti dalla classe 
        std::vector<Device*> device_list_;
        //Vettore che contiene i device attivi per ordine di accensione, necessario nel caso
        //in cui venga superato il limite di potenza e si debba spegnere i dispositivi in ordine
        //di accensione
        std::vector<Device*> device_insert_order_;
    
        //Variabile che permette di stampare sia a schermo, sia su un file .txt grazie alla classe
        //OutputManager
        OutputManager& out_;
        //Variabile che indica la massima potenza utilizzabile
        const double kMaxPower = 3.5; 
        //Tiene traccia di quanta potenza sia attualmente utilizzata dai dispositivi accesi
        double power_use_;
        //Variabile booleana che indica se la giornata è finita (Ovvero quando si arriva alle 23:59)
        //E' false quando non è finita, mentre è true quando è finita
        bool fine_giornata_;
        //Tempo del sistema
        CustomTime current_time_;
        //Numero di dispositivi da gestire.   
        int device_count_;
    
        //Funzione per debug: indica tutte le entry in active_devices_
        void PrintInfoAll(std::string_view rem) const;
        //Funzione per debug: indica tutte le entry in async_devices_
        void PrintInfoAsync(std::string_view rem) const;

};
//Prende come input una stringa composta da parole e spazi e un vettore di stringhe,
//e restituisce il vettore di stringhe riempito di entry delle parole separate dagli
//spazi
void SentenceIntoWords( std::vector<std::string>& ret, std::string sentence);
//Prende come input una stringa che rappresenta un orario, e restituisce l'orario
//corrispondente. Se il formato dell'orario è errato, lancia l'eccezione invalid_argument
CustomTime StringIntoCTime(std::string orario);
#endif