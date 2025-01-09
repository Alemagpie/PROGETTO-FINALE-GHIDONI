#include "../include/DeviceManager.h"

DeviceManager::DeviceManager()
    : deviceCount{0}, currentDevice{nullptr}, deviceList(), activeDevices(), asyncDevices()
{
    currentTime.setTime(0,0);
    currentDeviceEndTime.setTime(0,0);
}

void DeviceManager::addDevice(){
    CustomTime e_time = currentDeviceEndTime;
    activeDevices.insert(std::pair<CustomTime, Device*>(e_time, currentDevice)); //aggiungi entry con (chiave end_time e valore puntatore a d) alla multimappa dei device attivi
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
    std::istringstream iss(command);
    std::vector<std::string> words;
    SentenceIntoWords(words, command);
    //for(int i=0; i<words.size(); i++){
     //   std::cout << words[i] << std::endl;
    //}
        
    //std::string word;
    //std::getline(iss, word, ' ');
    //controllo della prima parola
    switch(firstToSwitch(words[0])){
        case firstCommand::set:
            
            break;

        case firstCommand::rm:
        if (words.size() == 2){    //"rm ${DEVICE}"      Rimuovere i timer associati ad un dispositivo.
                auto iter = findDeviceByNameAll(words[1]);
                if(iter == deviceList.end()) {std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;}
                else {
                    //(**iter).             //WIP, working on it :) -Ric
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

void DeviceManager::setTime(CustomTime& newTime) {
    currentTime = newTime;
    checkOnHourChange();
}

void DeviceManager::checkOnHourChange(){

}