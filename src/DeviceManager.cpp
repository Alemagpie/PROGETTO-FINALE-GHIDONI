#include "../include/DeviceManager.h"

DeviceManager::DeviceManager(){
    currentTime.setTime(0,0);
    currentDeviceEndTime.setTime(0,0);
}

void DeviceManager::addDevice(Device& d) {
    Time s_time = currentTime;
    Time e_time = currentDeviceEndTime;

    //d.startDevice(s_time, e_time);
    //activeDevices.insert(std::pair<Time, Device>(e_time, d));
    //aggiungi entry con (chiave key e valore d) alla multimappa dei device attivi
}

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
                        break;
                    case resetCommand::timersReset:
                        break;
                    case resetCommand::allReset:
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
}