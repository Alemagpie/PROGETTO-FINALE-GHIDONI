//DAVIDE MEROTTO

#ifndef PROGETTOFINALE_SRC_DEVICE_H
#define PROGETTOFINALE_SRC_DEVICE_H

#include <string>

#include "CustomTime.h"

//Classe base -> racchiude membri per la gestione di un dispositivo domotico
class Device{

    public: 
        //Disattivo costruttore di copia e operatore di assegnamento -> evito slicing
        Device(const Device&) = delete;
        Device& operator= (const Device&) = delete;

        inline void StopDevice()    {status_=false;}
        inline void StartDevice()   {status_=true;}
        inline std::string GetName() const  {return name_;}
        inline int GetID() const    {return id_;}
        inline bool GetStatus() const   {return status_;}
        inline CustomTime GetStartTime() const  {return start_time_;}
        inline CustomTime GetEndTime() const    {return end_time_;}
        inline const double GetCurrentPowerConsumption() const  {return power_consumption_;}
        //Restituisco il consumo attuale del dispositivo
        inline const double GetPowerUsed() const    {return power_used_;}
        //Aggiorna il consumo in un'orario qualsiasi della giornata
        void UpdatePowerUsed(const CustomTime& currentTime);
        //Resetta il consumo e lo stato del dispositivo
        void Reset();

        //Funzioni virtuali pure, rendono la classe astratta -> funzioni da sovrascrivere nelle sottoclassi

        virtual void UpdateStartTime(CustomTime newStartTime) = 0;
        virtual void UpdateEndTime(CustomTime newEndTime) = 0;
        virtual void UpdateEndTime() = 0;
        //Imposto tempo di inizio e di fine -> l'orario di fine viene considerato solo per i dispositivi manuali
        virtual void SetTimer(const CustomTime& start_t, CustomTime end_t ) = 0; 
        //Funzione per annullare eventuale stop time -> valido solamente per i dispositivi manuali
        //La gestione del timer è gestita direttamente nella classe Device Manager 
        virtual void RemoveTimer() = 0;
        //Restituisce il tipo di dispostivo tramite carattere
        virtual char GetType() const =0;
        
    protected:

        //Costruttore protected -> non permetto la creazione di oggetti di tipo Device (la classe è comunque astratta)
        Device(std::string nome, const int ID, double power);

        CustomTime start_time_;
        CustomTime end_time_;
        std::string name_;
        const int id_;
        double power_consumption_;
        double power_used_;  
        bool status_;      
};

//Helper functions

//Sovrascrivo operatore == per il confronto 
inline bool operator==(const Device& a, const Device& b)    {return (a.GetName() == b.GetName() && a.GetID() == b.GetID());}

//Sovrascrivo operatore << per la stampa
std::ostream& operator<<(std::ostream& os, Device& dev);

#endif