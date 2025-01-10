#include "../include/DeviceManager.h"

DeviceManager::DeviceManager()
    : deviceCount{0}, currentDevice{nullptr}, deviceList(), activeDevices(), asyncDevices()
{
    currentTime.setTime(0,0);
    currentDeviceEndTime.setTime(0,0);
}

void DeviceManager::addDevice(Device* dev){
    activeDevices.insert(std::pair<CustomTime, Device*>(dev->getEndTime(), dev)); //aggiungi entry con (chiave end_time e valore puntatore a d) alla multimappa dei device attivi
}

void DeviceManager::addDeviceToList(Device& newDev) {
    deviceList.push_back(&newDev);
    deviceCount++;
}

void DeviceManager::setDeviceStartTime() {  //rimosso parametro "Device* d" perchè è più semplice lavorare con la variabile currentDevice, settandola in parseInput()
    currentDevice->setTimer(currentTime, currentDeviceEndTime);
}

//metodo per comando "set ${devicename} off"
Device* DeviceManager::removeDevice(std::multimap<CustomTime, Device*>::iterator it) {
    Device* d = it->second;
    activeDevices.erase(it);
    return d;
} 


std::multimap<CustomTime, Device*>::iterator DeviceManager::findDevice(Device& d) {
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second == &d) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return activeDevices.end();
}

std::multimap<CustomTime, Device*>::iterator DeviceManager::findDeviceByID(int ID) {
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second->getID() == ID) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return activeDevices.end();
}

std::multimap<CustomTime, Device*>::iterator DeviceManager::findDeviceByNameActive(std::string& s) {
    //std::multimap<CustomTime, Device*>::iterator it;
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second->getName() == s) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return activeDevices.end();
}

std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator DeviceManager::findDeviceByNameAsync(std::string& s) {
    //std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it;
    for(auto it = asyncDevices.begin(); it != asyncDevices.end(); ++it) {
        if(it->second.second->getName() == s) {    //it->first restituisce il CustomTime, it->second restituisce Device*
            return it;
        }
    }
    return asyncDevices.end();
}

std::vector<Device*>::iterator DeviceManager::findDeviceByNameAll(std::string& s) {
    //std::vector<Device*>::iterator it;
    for(auto it = deviceList.begin(); it != deviceList.end(); ++it) {
        if((*it)->getName() == s) {  
            return it;
        }
    }
    return deviceList.end();
}

void SentenceIntoWords(std::vector<std::string>& ret, std::string sentence){
    int initpos=0, endpos=0;
    for(endpos; endpos<sentence.size(); endpos++){
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
        std::cout << key << value << ", ";
    std::cout << "}\n";
    std::cout << "Size=" << activeDevices.size() << '\n';
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
    currentDeviceEndTime = currentTime;
    //stream per fare il parsing del comando
    std::vector<std::string> words;
    SentenceIntoWords(words, command);
    //controllo della prima parola                  
    switch(firstToSwitch(words[0])){
        case firstCommand::set:
            if(words.size() >= 3 && words.size() < 5){      //Controllo che ci sia il giusto numero di parole nel comando
                try{
                    if(words[1] == "time"){
                        int newHour = std::stoi(words[2].substr(0, words[2].find(":")));        //Trasformo da string a int con la funzione stoi
                        int newMin = std::stoi(words[2].substr(words[2].find(":")+1));
                        CustomTime newTime(newHour, newMin);
                        std::cout<< newTime << std::endl;                       //TO DO: fare controllo dell'orario
                        setTime(newTime);
                    }else{
                        if(words[2] == "on"){                   //set ${DEVICE} on
                            auto iterAll = findDeviceByNameAll(words[1]);
                            auto iterActive = findDeviceByNameActive(words[1]);
                            if(iterAll != deviceList.end() && iterActive == activeDevices.end() ){
                                (*iterAll)->updateStartTime(currentTime);
                                (*iterAll)->updateEndTime();
                                addDevice(*iterAll);
                                print_infoAll("Multimappa attivi: ");
                            }else if(iterActive != activeDevices.end()){
                                std::cout << "Device gia attivo. Se si vuole modificare i suoi orari, spegnerlo e rirpovare."<< std::endl;
                            } else{
                                std::cout << "Device non riconosciuto. Fare attenzione al nome riportato." << std::endl;
                            }
                        }else if(words[2] == "off"){            //set ${DEVICE} off
                            auto iterAll = findDeviceByNameAll(words[1]);
                            auto iterActive = findDeviceByNameActive(words[1]);
                            if(iterAll != deviceList.end() && iterActive != activeDevices.end() ){
                                iterActive->second->updatePowerUsed(currentTime);
                                iterActive->second->stopDevice();
                                activeDevices.erase(iterActive);
                                print_infoAll("Multimappa attivi: ");
                            }else if(iterActive == activeDevices.end()){
                                std::cout << "Device non attivo. Prima di spegnere il dispositivo, e' necessario attivarlo."<< std::endl;
                            } else{
                                std::cout << "Device non riconosciuto. Fare attenzione al nome riportato." << std::endl;
                            }
                        }else{                                  //set ${DEVICE} ${START_TIME} ${END_TIME}
                            //TO DO
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
                }              
            } else {std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;}
            break;


        case firstCommand::show:
            if(words.size() == 1){  //"show ${devicename}"
                double totalPowerUsed=0;
                for(int i=0; i<deviceCount; i++){
                    std::cout << *deviceList[i] << std::endl;
                    totalPowerUsed += deviceList[i]->getPowerUsed();
                }
                std::cout << "Consumo energetico totale del sistema dalle 00:00 : " << totalPowerUsed << "kWh" << std::endl;
            } else if (words.size() == 2){    //"show"
                auto iter = findDeviceByNameAll(words[1]);
                if(iter == deviceList.end()) {std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;}
                else {std::cout<< **iter << std::endl;}              
            } else {std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;}
            break;
        

        case firstCommand::reset:
            switch(resetToSwitch(words[1])){
                case resetCommand::timeReset:   //"reset time"
                    activeDevices.clear();                              //TO DO: Aggiornare PowerUsed and Status, anche per altri reset.
                    currentTime.setTime(0,0);
                    break;
                case resetCommand::timersReset: //"reset timers"
                    //Rimuovi timer
                    asyncDevices.clear();
                    break;
                case resetCommand::allReset:    //"reset all"
                    //Spegni dispositivi attivi
                    activeDevices.clear();
                    //Rimuovi timer
                    asyncDevices.clear();
                    currentTime.setTime(0,0);
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

void DeviceManager::setTime(CustomTime newTime) {
    currentTime = newTime;
    checkOnHourChange();
}

void DeviceManager::checkOnHourChange(){

}