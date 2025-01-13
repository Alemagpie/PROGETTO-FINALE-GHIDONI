

#include "DeviceManager.h"
#include "CustomTime.h"
#include <iterator>
#include <utility> //per std::pair
#include <map>
#include "CustomTime.h"
#include "OutputManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

class Console {
    public:
    Console(DeviceManager& manager);
    void parseInput(const std::string command);   //valuta input

    private: 
    DeviceManager  devManager;
    std::multimap<CustomTime, Device*> activeMultimap; //multimappa dei dispositivi attivi
    std::multimap<CustomTime, std::pair<CustomTime, Device*>> asyncMultimap;  //multimappa dei dispositivi in attesa dell'attivazione (hanno come chiave il tempo di inizio, come valore il tempo di fine e il ptr al device)
    std::vector<Device*> ListOfDevices; //vettore di ptr ai Device
    std::vector<Device*> InsertOrderVector;
    int DeviceCount;
};

void SentenceIntoWords(std::vector<std::string>& ret, std::string sentence);