//ALESSIO FAGNANI

#include <iostream>
#include <vector>
#include <fstream>

#include "../include/AutomaticDevice.h"
#include "../include/ManualDevice.h"
#include "../include/DeviceManager.h"
#include "../include/CustomTime.h"
#include "../include/OutputManager.h"

using namespace std;

int main() {

    //Creazione file di log
    ofstream logFile("logEsempio.txt");

    if(!logFile.is_open()){
        //Segnalo errore
        cout<<"Errore apertura file";
        return 1; 
    }

    //Creo oggetto OutputManager per la gestione dell'output
    OutputManager out(cout, logFile);

    //Creo tutti i device nello stack
    ManualDevice Impianto_fotovoltaico("Impianto_fotovoltaico", 0, 1.5);
    ManualDevice Pompa_di_calore_termostato("Pompa_di_calore_termostato", 1, -2);
    ManualDevice Scaldabagno("Scaldabagno", 2, -1);
    ManualDevice Frigorifero("Frigorifero", 3, -0.4);
    AutomaticDevice Lavatrice("Lavatrice", 4, -2, CustomTime(1,50));
    AutomaticDevice Lavastoviglie("Lavastoviglie", 5, -1.5, CustomTime(3,15));
    AutomaticDevice Tapparelle_elettriche("Tapparelle_elettriche", 6, -0.3, CustomTime(0,1));
    AutomaticDevice Forno_a_microonde("Forno_a_microonde", 7, -0.8, CustomTime(0,2));
    AutomaticDevice Asciugatrice("Asciugatrice", 8, -0.5, CustomTime(1,0));
    AutomaticDevice Televisore("Televisore", 9, -0.2, CustomTime(1,0));

    //Gestore di tutti i dispositivi
    DeviceManager DevMan(out);

    //Aggiungo tutti i dispositivi nel vettore del DeviceManager
    DevMan.AddDeviceToList(Impianto_fotovoltaico);
    DevMan.AddDeviceToList(Pompa_di_calore_termostato);
    DevMan.AddDeviceToList(Scaldabagno);
    DevMan.AddDeviceToList(Frigorifero);
    DevMan.AddDeviceToList(Lavatrice);
    DevMan.AddDeviceToList(Lavastoviglie);
    DevMan.AddDeviceToList(Tapparelle_elettriche);
    DevMan.AddDeviceToList(Forno_a_microonde);
    DevMan.AddDeviceToList(Asciugatrice);
    DevMan.AddDeviceToList(Televisore);

    //Input del comando
    while(!DevMan.GetFineGiornata()){
        string s; 
        getline(cin, s);
        DevMan.ParseInput(s);
    }

    return 0;
}