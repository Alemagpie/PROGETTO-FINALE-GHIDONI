#include "../include/DeviceManager.h"

DeviceManager::DeviceManager(){
    currentTime.setTime(0,0);
    currentDeviceEndTime.setTime(0,0);
}


void DeviceManager::addDevice() {  //rimosso parametro "Device* d" perchè è più semplice lavorare con la variabile currentDevice, settandola in parseInput()
    Time s_time = currentTime;
    Time e_time = currentDeviceEndTime;

    activeDevices.insert(std::pair<Time, Device*>(e_time, currentDevice)); //aggiungi entry con (chiave end_time e valore puntatore a d) alla multimappa dei device attivi
}

//metodo per comando "set ${devicename} off"
Device* DeviceManager::removeDevice(std::multimap<Time, Device*>::iterator it) {
    Device* d = it->second;
    activeDevices.erase(it);

    return d;
} 



std::multimap<Time, Device*>::iterator DeviceManager::findDevice(Device& d) {
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second == &d) {    //it->first restituisce il Time, it->second restituisce Device*
            return it;
        }
    }
}

std::multimap<Time, Device*>::iterator DeviceManager::findDeviceByID(int ID) {
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second->getID() == ID) {    //it->first restituisce il Time, it->second restituisce Device*
            return it;
        }
    }
}

std::multimap<Time, Device*>::iterator DeviceManager::findDeviceByName(std::string& s) {
    //std::multimap<Time, Device*>::iterator it;
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        if(it->second->getName() == s) {    //it->first restituisce il Time, it->second restituisce Device*
            return it;
        }
    }
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
    std::string word;
    std::getline(iss, word, ' ');
    //controllo della prima parola
    switch(firstToSwitch(word)){
        case firstCommand::set:
            std::getline(iss, word, ' ');
            break;

        case firstCommand::rm:
            break;

        case firstCommand::show:
            break;

        case firstCommand::reset:
             if(std::getline(iss, word, ' ')){
                switch(resetToSwitch(word)){
                    case resetCommand::timeReset:
                        activeDevices.clear();
                        currentTime.setTime(0,0);
                        break;
                    case resetCommand::timersReset:
                        //Rimuovi timer
                        asyncDevices.clear();
                        break;
                    case resetCommand::allReset:
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
             }
            break;

        default:
            std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;
            break;
    }
    
}

void DeviceManager::setTime(Time& newTime) {
    currentTime = newTime;
    checkOnHourChange();
}