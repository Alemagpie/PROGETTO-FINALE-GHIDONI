#include "../include/AutomaticDevice.h"
#include "../include/ManualDevice.h"
#include "../include/DeviceManager.h"
#include "../include/CustomTime.h"
#include "../include/OutputManager.h"
#include "../include/Console.h"
#include <iostream>
#include <vector>
#include <fstream>  //file stream

using namespace std;

int main() {

    //creazione file di log
    ofstream logFile("logEsempio.txt");

    if(!logFile.is_open()){
        cout<<"Errore apertura file";
        return 1; //segnalo errore
    }

    //creo oggetto OutputManager per la gestione dell'output
    OutputManager out(cout, logFile);

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

    DeviceManager DevMan(out);
    Console c(DevMan);

    DevMan.addDeviceToList(Impianto_fotovoltaico);
    DevMan.addDeviceToList(Pompa_di_calore_termostato);
    DevMan.addDeviceToList(Scaldabagno);
    DevMan.addDeviceToList(Frigorifero);
    DevMan.addDeviceToList(Lavatrice);
    DevMan.addDeviceToList(Lavastoviglie);
    DevMan.addDeviceToList(Tapparelle_elettriche);
    DevMan.addDeviceToList(Forno_a_microonde);
    DevMan.addDeviceToList(Asciugatrice);
    DevMan.addDeviceToList(Televisore);

    //gestione dei comandi
    while(!DevMan.getFineGiornata()){
        string s; 
        getline(cin, s);
        c.parseInput(s);
    }

    return 0;
}