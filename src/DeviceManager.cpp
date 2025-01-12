#include "../include/DeviceManager.h"

DeviceManager::DeviceManager()
    : deviceCount{0}, deviceList(), activeDevices(), asyncDevices(), powerUse{0}
{
    currentTime.setTime(0,0);
    std::cout<<"Device Manager accesso"<<std::endl;
}

void DeviceManager::addDevice(Device* dev){
    if(checkPowerConsumption(dev)) {
        dev->updateStartTime(currentTime);
        activeDevices.insert(std::pair<CustomTime, Device*>(dev->getEndTime(), dev)); //aggiungi entry con (chiave end_time e valore puntatore a d) alla multimappa dei device attivi
        dev->setStatus(true);
        powerUse += dev->getCurrentPowerConsumption();
        std::cout << "[" << currentTime << "] Il dispositivo \'" << dev->getName() << "\' si e' acceso" << std::endl;  
    } else {
        std::cout<<"Superata soglia di consumo. E' stato spento il dispositivo: "<<dev->getName()<<std::endl;
    }
    
}

void DeviceManager::addDeviceToList(Device& newDev) {
    deviceList.push_back(&newDev);
    deviceCount++;
}

void DeviceManager::addDeviceAsync(Device* dev, CustomTime Start, CustomTime End){
    if(Start > currentTime) {
    asyncDevices.insert(std::pair<CustomTime, std::pair<CustomTime, Device*>>(Start, std::pair<CustomTime, Device*>(End, dev))); 
    } else {
        std::cout<<"Orario di inizio non valido";
    }
}

void DeviceManager::moveDevice(std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it) {
    std::pair<CustomTime, Device*> deviceToMove = it->second;
    deviceToMove.second->updateEndTime(deviceToMove.first);
    print_infoAsync("Prova ");
    asyncDevices.erase(it);
    print_infoAsync("Prova ");
    addDevice(deviceToMove.second);
    print_infoAsync("Prova ");
}

//metodo per comando "set ${devicename} off"
Device* DeviceManager::removeDevice(std::multimap<CustomTime, Device*>::iterator it) {
    Device* d = it->second;
    activeDevices.erase(it);
    d->updatePowerUsed(currentTime);
    d->setStatus(false);
    std::cout << "[" << currentTime << "] Il dispositivo \'" << d->getName() << "\' si e' spento" << std::endl;

    powerUse -= d->getCurrentPowerConsumption();
    return d;
} 


std::multimap<CustomTime, Device*>::iterator DeviceManager::findDevice(Device& d) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&d](const std::pair<CustomTime, Device*>& element) -> bool {
            return *(element.second) == d;
        });

    /*
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second == &d) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return activeDevices.end();
    */
}

std::multimap<CustomTime, Device*>::iterator DeviceManager::findDeviceByID(int ID) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&ID](const std::pair<CustomTime, Device*>& element) -> bool {
            return element.second->getID() == ID;
        });
    
    /*
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second->getID() == ID) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return activeDevices.end();
    */
}

std::multimap<CustomTime, Device*>::iterator DeviceManager::findDeviceByNameActive(std::string& s) {
    return std::find_if(activeDevices.begin(), activeDevices.end(),
        [&s](const std::pair<CustomTime, Device*>& element) -> bool {
            return element.second->getName() == s;
        });
    
    /*
    //std::multimap<CustomTime, Device*>::iterator it;
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second->getName() == s) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return activeDevices.end();
    */
}

std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator DeviceManager::findDeviceByNameAsync(std::string& s) {
    return std::find_if(asyncDevices.begin(), asyncDevices.end(),
        [&s](const std::pair<CustomTime, std::pair<CustomTime, Device*>>& element) -> bool {
            return element.second.second->getName() == s;
        });

    /*
    //std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it;
    for(auto it = asyncDevices.begin(); it != asyncDevices.end(); ++it) {
        if(it->second.second->getName() == s) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return asyncDevices.end();
    */
}

std::vector<Device*>::iterator DeviceManager::findDeviceByNameAll(std::string& s) {
    return std::find_if(deviceList.begin(), deviceList.end(),
        [&s](Device* d) -> bool {
            return d->getName() == s;
        });

    /*
    //std::vector<Device*>::iterator it;
    for(auto it = deviceList.begin(); it != deviceList.end(); ++it) {
        if((*it)->getName() == s) {  
            return it;
        }
    }
    return deviceList.end();
    */
}

void SentenceIntoWords(std::vector<std::string>& ret, std::string sentence){
    int initpos=0;
    for(int endpos=0; endpos<sentence.size(); endpos++){
        if(sentence[endpos] == ' '){
            ret.push_back(sentence.substr(initpos, endpos-initpos));
            initpos = endpos+1;
        } 
    }
    ret.push_back(sentence.substr(initpos));
}

//DA MODIFICARE
void DeviceManager::print_infoAll(std::string_view rem)
{
    std::cout << rem << "{ ";
    for (const auto& [key, value] : activeDevices)
        std::cout << key << *value << ", ";
    std::cout << "}\n";
    std::cout << "Size=" << activeDevices.size() << '\n';
}

void DeviceManager::print_infoAsync(std::string_view rem)
{
    std::cout << rem << "{ ";
    for (const auto& [key, value] : asyncDevices)
        std::cout << key << value.first << *(value.second) <<", ";
    std::cout << "}\n";
    std::cout << "Size=" << asyncDevices.size() << '\n';
}

//--------------------------------------------------------------------------

enum firstCommand{
    baseFirst = 0,set, rm, show, reset
};

firstCommand firstToSwitch(std::string& command){
    if( command == "set") return firstCommand::set;
    if( command == "rm") return firstCommand::rm;
    if( command == "show") return firstCommand::show;
    if( command == "reset") return firstCommand::reset;
    return firstCommand::baseFirst;
}

enum resetCommand{
    baseReset=0, timeReset, timersReset, allReset
};

resetCommand resetToSwitch(std::string& command){
    if( command == "time") return resetCommand::timeReset;
    if( command == "timers") return resetCommand::timersReset;
    if( command == "all") return resetCommand::allReset;
    return resetCommand::baseReset;
}


void DeviceManager::parseInput(std::string command){
    std::cout<< "["<< currentTime << "] L'orario attuale e' " << currentTime << std::endl;
    //stream per fare il parsing del comando
    std::vector<std::string> words;
    SentenceIntoWords(words, command);
    //controllo della prima parola                  
    switch(firstToSwitch(words[0])){
        case firstCommand::set:
            if(words.size() >= 3 && words.size() < 5){      //Controllo che ci sia il giusto numero di parole nel comando
                try{
                    if(words[1] == "time"){ //set time xx:yy
                        int newHour = std::stoi(words[2].substr(0, words[2].find(":")));        //Trasformo da string a int con la funzione stoi
                        int newMin = std::stoi(words[2].substr(words[2].find(":")+1));
                        CustomTime newTime(newHour, newMin);
                        //std::cout<< newTime << std::endl;                       //TO DO: fare controllo dell'orario
                        setTime(newTime);
                    }else{
                        if(words[2] == "on"){                   //set ${DEVICE} on
                            auto iterAll = findDeviceByNameAll(words[1]);
                            auto iterActive = findDeviceByNameActive(words[1]);

                            if (iterAll != deviceList.end() && *iterAll != nullptr) {
                                if (iterActive == activeDevices.end()) {
                                    (*iterAll)->updateStartTime(currentTime);
                                    (*iterAll)->updateEndTime();
                                    addDevice(*iterAll);
                                    //print_infoAll("Multimappa attivi: ");
                                } else {
                                    std::cout << "Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." << std::endl;
                                }
                            } else if (iterActive != activeDevices.end()) {
                                std::cout << "Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." << std::endl;
                            } else {
                                std::cout << "Device non riconosciuto. Fare attenzione al nome riportato." << std::endl;
                            }
                            /*if(iterAll != deviceList.end() && iterActive == activeDevices.end() ){
                                (*iterAll)->updateStartTime(currentTime);
                                (*iterAll)->updateEndTime();
                                std::cout << "Prova" << std::endl;
                                addDevice(*iterAll);
                                std::cout << "Prova 2" << std::endl;
                                print_infoAll("Multimappa attivi: ");
                            }else if(iterActive != activeDevices.end()){
                                std::cout << "Device gia' attivo. Se si vuole modificare i suoi orari, spegnerlo e rirpovare."<< std::endl;
                            } else{
                                std::cout << "Device non riconosciuto. Fare attenzione al nome riportato." << std::endl;
                            }*/
                        }else if(words[2] == "off"){            //set ${DEVICE} off
                            auto iterAll = findDeviceByNameAll(words[1]);
                            auto iterActive = findDeviceByNameActive(words[1]);
                            if(iterAll != deviceList.end() && iterActive != activeDevices.end() ){  
                                iterActive->second->updatePowerUsed(currentTime);
                                iterActive->second->stopDevice();
                                activeDevices.erase(iterActive);
                                //removeDevice(iterActive);
                                //print_infoAll("Multimappa attivi: ");
                            }else if(iterActive == activeDevices.end()){
                                std::cout << "Device non attivo. Prima di spegnere il dispositivo, e' necessario attivarlo."<< std::endl;
                            } else{
                                std::cout << "Device non riconosciuto. Fare attenzione al nome riportato." << std::endl;
                            }
                        }else{                                  //set ${DEVICE} ${START_TIME} ${END_TIME}
                            auto iterAll = findDeviceByNameAll(words[1]);
                            auto iterActive = findDeviceByNameActive(words[1]);
                            auto iterAsync = findDeviceByNameAsync(words[1]);
                            if(iterAll != deviceList.end()){
                                int startHour = std::stoi(words[2].substr(0, words[2].find(":")));        //Trasformo da string a int con la funzione stoi
                                int startMin = std::stoi(words[2].substr(words[2].find(":")+1));
                                CustomTime timeStart(startHour,startMin);
                                if(words.size()==4){                                              //Se c'è il parametro END_TIME
                                     int endHour = std::stoi(words[3].substr(0, words[3].find(":")));
                                    int endMin = std::stoi(words[3].substr(words[3].find(":")+1));
                                    CustomTime timeEnd(endHour, endMin);
                                    addDeviceAsync(*iterAll, timeStart, timeEnd);
                                    std::cout<< "["<< currentTime << "] Impostato un timer per il dispositivo \'" << (*iterAll)->getName() <<"\' dalle "<< timeStart << " alle " << timeEnd <<std::endl;
                                }else{
                                    addDeviceAsync(*iterAll, timeStart, (*iterAll)->getEndTime());
                                    std::cout<< "["<< currentTime << "] Impostato un timer per il dispositivo \'" << (*iterAll)->getName() <<"\' dalle "<< timeStart  <<std::endl;
                                }
                                //print_infoAsync("Async attivi: ");
                            }else{
                                std::cout << "Device non riconosciuto. Fare attenzione al nome riportato." << std::endl;
                            }
                        }
                    }
                }catch(std::exception e){
                    std::cerr<<e.what() << std::endl;
                }
            }else{
                std::cout<< "Comando non riconosciuto. Riprovare." << std::endl;
            }
            break;


        case firstCommand::rm:
        if (words.size() == 2){    //"rm ${DEVICE}"      Rimuovere i timer associati ad un dispositivo.
                auto iterAll = findDeviceByNameAll(words[1]);
                if(iterAll == deviceList.end()) {std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;}
                else {
                    auto iterAsync = findDeviceByNameAsync(words[1]);
                    iterAsync->second.second->removeTimer();                //DA CONTROLLARE QUANDO VIENE MESSO L'ADD
                    asyncDevices.erase(iterAsync);
                    std::cout<< "["<< currentTime << "] Rimosso il timer dal dispositivo \'" << (*iterAll)->getName() <<"\'" << std::endl;
                }              
            } else {std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;}
            break;


        case firstCommand::show:
            //aggiorno il consumo
            for(auto it = activeDevices.begin();it != activeDevices.end(); ++it){
                    it->second->updatePowerUsed(currentTime);
            }
            if(words.size() == 1){  //"show "
                double totalPowerUsed=0;
                for(int i=0; i<deviceCount; i++){
                    std::cout << *deviceList[i] << std::endl;
                    totalPowerUsed += deviceList[i]->getPowerUsed();
                }
                std::cout << "Consumo energetico totale del sistema dalle 00:00 : " << totalPowerUsed << "kWh" << std::endl;
            } else if (words.size() == 2){    //"show ${devicename}"
                auto iter = findDeviceByNameAll(words[1]);
                if(iter == deviceList.end()) {std::cout<<"Device non riconosciuto. Riprovare." << std::endl;}
                else {std::cout<< "Il dispositivo \'" << (*iter)->getName()<< "\' ha attualmente consumato "<< (*iter)->getPowerUsed()<< " kWh" << std::endl;}              
            } else {std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;}
            break;
        

        case firstCommand::reset:
            switch(resetToSwitch(words[1])){
                case resetCommand::timeReset:   //"reset time"
                    activeDevices.clear();                              //TO DO: Aggiornare PowerUsed and Status, anche per altri reset. CHECK
                    setTime(CustomTime(0,0));
                    for(int i=0; i<deviceCount; i++){deviceList[i]->reset();}

                    break;
                case resetCommand::timersReset: //"reset timers"
                    asyncDevices.clear();                                //Rimuovi timer
                    break;
                case resetCommand::allReset:    //"reset all"
                    activeDevices.clear();                          //Spegni dispositivi attivi
                    asyncDevices.clear();                              //Rimuovi timer
                    setTime(CustomTime(0,0));
                    for(int i=0; i<deviceCount; i++){deviceList[i]->reset();}
                    break;
                default:
                    std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;
                    break;
                }
            break;

        default:
            std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;
    }
    
}

void DeviceManager::setTime(CustomTime newTime) {                 //Controllo tempi di start, end , e che non ci siano conflitti
    auto asyncIt = asyncDevices.begin();
    auto activeIt = activeDevices.begin();
    while (asyncIt != asyncDevices.end() || activeIt != activeDevices.end())
    {
        //std::cout << "ProvaGen" << std::endl;
        if(asyncIt == asyncDevices.end()){      //Guardo solo gli attivi
            if(activeIt->first <= newTime) {
                //std::cout << currentTime <<newTime << activeIt->first<<std::endl;
                currentTime = activeIt->first;

                auto activeItRemove = activeIt;
                activeIt++;
                removeDevice(activeItRemove);
            } else{
                activeIt = activeDevices.end();
            }
        } else {
            if (activeIt == activeDevices.end()){ //Guardo solo gli asincroni
                if(asyncIt->first <= newTime) {
                    //std::cout << "ProvaAsyn" << std::endl;
                    currentTime = asyncIt->first;

                    auto asyncItRemove = asyncIt;
                    asyncIt++;
                    moveDevice(asyncItRemove);
                } else{
                    asyncIt = asyncDevices.end();
                }
        } else {        //guardo entrambi
            std::cout<<"Rimozione da entrambi"<<std::endl;
            //std::cout << "ProvaEntrambi" << std::endl;
            if(asyncIt->first <= activeIt->first && asyncIt->first <= newTime){
                currentTime = asyncIt->first;

                auto asyncItRemove = asyncIt;
                asyncIt++;
                moveDevice(asyncItRemove);
            } else {
                if(activeIt->first <= asyncIt->first && activeIt->first <= newTime){
                
                auto activeItRemove = activeIt;
                activeIt++;
                removeDevice(activeItRemove);
                } else{
                    activeIt = activeDevices.end();
                    asyncIt = asyncDevices.end();
                }
            }
        }
        }
    }
    //std::cout << "ProvaFinale" << std::endl;
    currentTime = newTime;
    std::cout<< "["<< currentTime << "] L'orario attuale e' " << currentTime << std::endl;
}

bool DeviceManager::checkPowerConsumption(Device* d) {
    double currentDeviceConsumption = d->getCurrentPowerConsumption();
    
    //usiamo il > perchè il consumo è pensato in negativo
    return (currentDeviceConsumption + powerUse + maxPower >= 0);
}

double DeviceManager::checkPowerConsumptionGeneral() {
    double currentPower = 0;
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        currentPower += it->second->getCurrentPowerConsumption();
    }
    return currentPower;
}

/*
void DeviceManager::checkOnHourChange(){
    auto asyncIt = asyncDevices.begin();
    auto activeIt = activeDevices.begin();

    while ((asyncIt != asyncDevices.end() || activeIt != activeDevices.end())
    && (asyncIt->first <= currentTime || activeIt->first <= currentTime))
    {
        if(asyncIt->first <= activeIt->first && asyncIt->first <= currentTime) {
            moveDevice(asyncIt);
            asyncIt++;
        }

        if(activeIt != activeDevices.end() && activeIt->first <= currentTime && activeIt->first <= asyncIt-> first) {
            removeDevice(activeIt);
            activeIt++;
        }
    }
    currentTime = newTime;
}
*/

/*
void DeviceManager::setTime(CustomTime newTime) {                 //Controllo tempi di start, end , e che non ci siano conflitti
    auto asyncIt = asyncDevices.begin();
    auto activeIt = activeDevices.begin();
    std::cout << "Prova" << std::endl;
    while (asyncIt != asyncDevices.end() || activeIt != activeDevices.end())
    {
        if(asyncIt == asyncDevices.end()){      //Guardo solo gli attivi
            if(activeIt != activeDevices.end() && activeIt->first <= currentTime && activeIt->first <= asyncIt-> first) {
                currentTime = activeIt->first;
                removeDevice(activeIt);
                activeIt++;
            }
        } else if (activeIt == activeDevices.end()){ //Guardo solo gli asincroni

        } else {        //guardo entrambi

        }
        if(asyncIt->first <= currentTime || activeIt->first <= currentTime) {
                if(asyncIt->first <= activeIt->first && asyncIt->first <= currentTime) {
                    currentTime = asyncIt->first;
                    moveDevice(asyncIt);
                    asyncIt++;
                }

                if(activeIt != activeDevices.end() && activeIt->first <= currentTime && activeIt->first <= asyncIt-> first) {
                    currentTime = activeIt->first;
                    removeDevice(activeIt);
                    activeIt++;
                }
        }
    }
    currentTime = newTime;
}
*/