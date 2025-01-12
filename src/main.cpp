#include "../include/AutomaticDevice.h"
#include "../include/ManualDevice.h"
#include "../include/DeviceManager.h"
#include "../include/CustomTime.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main() {
    
    //Creazione file di log
    std:: string logFileName = "log.txt";
    //Creo stream di output file
    std:: ofstream logFile(logFileName);

    if(!logFile.is_open()){
        std::cout <<"Errore nell'apertura del file di log"<<endl;
        return 1;
    };
    //salvo buffer associato a cout
    std:: streambuf* coutBuffer = std:: cout.rdbuf();   //rdbuf restituisce un puntatore al buffer
    //reindirizzo cout al buffer associato alla stream del file
    std:: cout.rdbuf(logFile.rdbuf()); //ora cout scrive sul file e non sul terminale
    std::cout<< "Esempio file di log : \n"<<endl;

    ManualDevice Impianto_fotovoltaico("Impianto_Fotovoltaico", 0, 1.5);
    ManualDevice Pompa_di_calore_termostato("Pompa_di_calore_termostato", 1, -2);
    ManualDevice Scaldabagno("Scaldabagno", 2, -1);
    ManualDevice Frigorifero("Frigorifero", 3, -0.4);
    AutomaticDevice Lavatrice("Lavatrice", 4, -2, CustomTime(1,50));
    AutomaticDevice Lavastoviglie("Lavastoviglie", 5, -1.5, CustomTime(3,15));
    AutomaticDevice Tapparelle_elettriche("Tapparelle_elettriche", 6, -0.3, CustomTime(0,1));
    AutomaticDevice Forno_a_microonde("Forno_a_microone", 7, -0.8, CustomTime(0,2));
    AutomaticDevice Asciugatrice("Asciugatrice", 8, -0.5, CustomTime(1,0));
    AutomaticDevice Televisore("Televisore", 9, -0.2, CustomTime(1,0));

    DeviceManager DevMan;
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


    
    for (string s; getline(cin, s);){
        DevMan.parseInput(s);
    }
    
    //ripristina buffer
    std:: cout.rdbuf(coutBuffer);
    logFile.close();

    /*
    vector<Device*> container(10);
    container[0] = &Impianto_Fotovoltaico;
    container[1] = &Pompa_di_calore_termostato;
    container[2] = &Scaldabagno;
    container[3] = &Frigorifero;
    container[4] = &Lavatrice;
    container[5] = &Lavastoviglie;
    container[6] = &Tapparelle_elettriche;
    container[7] = &Forno_a_microone;
    container[8] = &Asciugatrice;
    container[9] = &Televisore;

    for(int i=0; i<container.size(); i++){
        cout << container[i]->getName() << " " << container[i]->getStatus() << " " << container[i]->getID() << endl;
    }
    */
    return 0;
}