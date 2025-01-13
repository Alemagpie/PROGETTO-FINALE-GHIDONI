#include "../include/DeviceManager.h"

#include <iterator>
#include <utility> //per std::pair
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

#include "../include/ManualDevice.h"
#include "../include/AutomaticDevice.h"
#include "../include/CustomTime.h"
#include "../include/OutputManager.h"

DeviceManager::DeviceManager(OutputManager& outPut)
    : deviceCount{0}, deviceList(), activeDevices(), asyncDevices(), powerUse{0}, deviceInsertOrder(), fineGiornata(false), out{outPut}
{
    currentTime.setTime(0,0);
    out<<"Device Manager accesso"<<"\n";
}

void DeviceManager::addDevice(Device* dev){
    if(checkPowerConsumption(dev)) {
        dev->updateStartTime(currentTime);
        activeDevices.insert(std::pair<CustomTime, Device*>(dev->getEndTime(), dev)); //aggiungi entry con (chiave end_time e valore puntatore a d) alla multimappa dei device attivi
        deviceInsertOrder.push_back(dev);
        dev->startDevice();
        powerUse += dev->getCurrentPowerConsumption();
        out << "[" << currentTime << "] Il dispositivo \'" << dev->getName() << "\' si e' acceso" <<"\n";  
    } else {
        out<< "[" << currentTime << "] Superata soglia di consumo. E' stato spento il dispositivo: "<<dev->getName()<<"\n";
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
        out<< "[" << currentTime << "] Orario di inizio non valido";
    }
}

void DeviceManager::moveDevice(std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it) {
    std::pair<CustomTime, Device*> deviceToMove = it->second;
    asyncDevices.erase(it);
    if(!(deviceToMove.second->getStatus())){
        deviceToMove.second->updateEndTime(deviceToMove.first);
        addDevice(deviceToMove.second);
    }else{
        out<< "[" << currentTime << "] Il device e' gia' attivo." <<"\n";
    }
    deviceToMove.second->updateEndTime(deviceToMove.first);
}

//metodo per comando "set ${devicename} off"
void DeviceManager::removeDevice(std::multimap<CustomTime, Device*>::iterator it) {
    Device* d = it->second;
    activeDevices.erase(it);
    d->updatePowerUsed(currentTime);
    d->stopDevice();
    out << "[" << currentTime << "] Il dispositivo \'" << d->getName() << "\' si e' spento" <<"\n";
    deviceInsertOrder.erase(std::find(deviceInsertOrder.begin(), deviceInsertOrder.end(), d));

    powerUse -= d->getCurrentPowerConsumption();
    if(powerUse<(maxPower * (-1))) {
        std::string removeLast = deviceInsertOrder.back()->getName();
        removeDevice(findDeviceByNameActive(activeDevices, removeLast));
        }
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
    out << rem << "{ ";
    for (const auto& [key, value] : activeDevices)
        out << key << *value << ", ";
    out << "}\n";
    out << "Size=" << activeDevices.size() << '\n';
}

void DeviceManager::print_infoAsync(std::string_view rem)
{
    out << rem << "{ ";
    for (const auto& [key, value] : asyncDevices)
        out << key << value.first << *(value.second) <<", ";
    out << "}\n";
    out << "Size=" << asyncDevices.size() << '\n';
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
    out<< "["<< currentTime << "] L'orario attuale e' " << currentTime <<"\n";      
    //stream per fare il parsing del comando
    std::vector<std::string> words;
    SentenceIntoWords(words, command);
    //controllo della prima parola                  
    switch(firstToSwitch(words[0])){
        case firstCommand::set:
            if(words.size() >= 3 && words.size() < 5){      //Controllo che ci sia il giusto numero di parole nel comando
                try{
                    if(words[1] == "time"){ //set time xx:yy
                        if(std::find(words[2].begin(), words[2].end(), ':') == words[2].end()) {throw std::invalid_argument("");}
                        int newHour = std::stoi(words[2].substr(0, words[2].find(":")));        //Trasformo da string a int con la funzione stoi
                        int newMin = std::stoi(words[2].substr(words[2].find(":")+1));
                        CustomTime newTime(newHour, newMin);
                        if(newTime > currentTime) {setTime(newTime);}
                        else{ out << "[" <<currentTime << "] Orario non disponibile. Inserire solo orari successivi a quello attuale." <<"\n";}
                    }else{
                        if(words[2] == "on"){                   //set ${DEVICE} on
                            auto iterAll = findDeviceByNameAll(deviceList, words[1]);
                            auto iterActive = findDeviceByNameActive(activeDevices, words[1]);

                            if (iterAll != deviceList.end() && *iterAll != nullptr) {
                                if (iterActive == activeDevices.end()) {
                                    (*iterAll)->updateStartTime(currentTime);
                                    (*iterAll)->updateEndTime();
                                    addDevice(*iterAll);
                                } else {
                                    out << "[" << currentTime << "] Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." <<"\n";
                                }
                            } else if (iterActive != activeDevices.end()) {
                                out << "[" << currentTime << "] Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." <<"\n";
                            } else {
                                out << "[" << currentTime << "] Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        }else if(words[2] == "off"){            //set ${DEVICE} off
                            auto iterAll = findDeviceByNameAll(deviceList, words[1]);
                            auto iterActive = findDeviceByNameActive(activeDevices, words[1]);
                            if(iterAll != deviceList.end() && iterActive != activeDevices.end() ){  
                                removeDevice(iterActive);
                                //print_infoAll("Multimappa attivi: ");
                            }else if(iterActive == activeDevices.end()){
                                out << "[" << currentTime << "] Device non attivo. Prima di spegnere il dispositivo, e' necessario attivarlo."<<"\n";
                            } else{
                                out << "[" << currentTime << "]Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        }else{                                  //set ${DEVICE} ${START_TIME} ${END_TIME}
                            auto iterAll = findDeviceByNameAll(deviceList, words[1]);
                            auto iterActive = findDeviceByNameActive(activeDevices, words[1]);
                            auto iterAsync = findDeviceByNameAsync(asyncDevices, words[1]);
                            if(iterAll != deviceList.end()){
                                if(std::find(words[2].begin(), words[2].end(), ':') == words[2].end()) {throw std::invalid_argument("");}
                                int startHour = std::stoi(words[2].substr(0, words[2].find(":")));        //Trasformo da string a int con la funzione stoi
                                int startMin = std::stoi(words[2].substr(words[2].find(":")+1));
                                CustomTime timeStart(startHour,startMin);
                                if(timeStart > currentTime) {
                                    if(words.size()==4 && (*iterAll)->getType() == 'M'){                                              //Se c'è il parametro END_TIME
                                        int endHour = std::stoi(words[3].substr(0, words[3].find(":")));
                                        int endMin = std::stoi(words[3].substr(words[3].find(":")+1));
                                        CustomTime timeEnd(endHour, endMin);
                                        addDeviceAsync(*iterAll, timeStart, timeEnd);
                                        out<< "["<< currentTime << "] Impostato un timer per il dispositivo \'" << (*iterAll)->getName() <<"\' dalle "<< timeStart << " alle " << timeEnd <<"\n";
                                    }else{
                                        addDeviceAsync(*iterAll, timeStart, (*iterAll)->getEndTime());
                                        out<< "["<< currentTime << "] Impostato un timer per il dispositivo \'" << (*iterAll)->getName() <<"\' dalle "<< timeStart  <<"\n";
                                    }
                                }
                                else{ out << "[" <<currentTime << "] Orario non disponibile. Inserire solo orari successivi a quello attuale." <<"\n";}
                            }else{
                                out << "Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        }
                    }
                }catch(std::invalid_argument e){
                    out<< "Orario inserito non valido. Riprovare."<<"\n";
                }
            }else{
                out<< "Comando non riconosciuto. Riprovare." <<"\n";
            }
            break;


        case firstCommand::rm:
        if (words.size() == 2){    //"rm ${DEVICE}"      Rimuovere i timer associati ad un dispositivo.
                auto iterAll = findDeviceByNameAll(deviceList, words[1]);
                if(iterAll == deviceList.end()) {out<< "[" << currentTime << "] Comando non riconosciuto. Riprovare." <<"\n";}
                else {
                    auto iterAsync = findDeviceByNameAsync(asyncDevices, words[1]);
                    auto iterActive = findDeviceByNameActive(activeDevices, words[1]);
                    (*iterAll)->removeTimer();  
                    if(iterAsync != asyncDevices.end()){
                        asyncDevices.erase(iterAsync);              //DA SISTEMARE FORSE
                    }                         
                    out<< "["<< currentTime << "] Rimosso il timer dal dispositivo \'" << (*iterAll)->getName() <<"\'" <<"\n";
                    if(iterActive != activeDevices.end()){
                        if ((*iterActive).first == (*iterActive).second->getEndTime()){
                            activeDevices.erase(iterActive);
                            activeDevices.insert(std::pair<CustomTime, Device*>((*iterAll)->getEndTime(), *iterAll));
                        }
                    }
                }              
            } else {out<< "[" << currentTime << "] Comando non riconosciuto. Riprovare." <<"\n";}
            break;


        case firstCommand::show:
            //aggiorno il consumo
            for(auto it = activeDevices.begin();it != activeDevices.end(); ++it){
                    it->second->updatePowerUsed(currentTime);
            }
            if(words.size() == 1){  //"show "
                double totalPowerUsed=0;
                double totalPowerDevices = checkPowerConsumptionGeneral();
                for(int i=0; i<deviceCount; i++){
                    if(deviceList[i]->getPowerUsed() < 0) {totalPowerUsed += deviceList[i]->getPowerUsed();}
                }
                out << "[" << currentTime << "] Attualmente il sistema ha consumato " << std::fixed << totalPowerUsed << std::setprecision(2) <<"kWh e la potenza massima accumulata fra i dispositivi accesi è: "<<totalPowerDevices<<". Nello specifico:"<<"\n";
                for(int i=0; i<deviceCount; i++){
                    if(deviceList[i]->getCurrentPowerConsumption() < 0) {out << "\t - Il dispositivo \'" << deviceList[i]->getName() << "\' ha una potenza di "<<deviceList[i]->getCurrentPowerConsumption()<<" e ha consumato " << deviceList[i]->getPowerUsed() << std::fixed << std::setprecision(2)<<" kWh"<<"\n";}
                    else{out << "\t - Il dispositivo \'" << deviceList[i]->getName() << "\' ha una potenza di "<<deviceList[i]->getCurrentPowerConsumption()<< " e ha prodotto " << std::fixed <<deviceList[i]->getPowerUsed() << std::setprecision(2) <<" kWh"<<"\n";}
                    
                }
            } else if (words.size() == 2){    //"show ${devicename}"
                auto iter = findDeviceByNameAll(deviceList, words[1]);
                if(iter == deviceList.end()) {out<<"Device non riconosciuto. Riprovare." <<"\n";}
                else {
                    if((*iter)->getCurrentPowerConsumption() < 0) {out << "\t - Il dispositivo \'" << (*iter)->getName() << "\'ha una potenza di "<<(*iter)->getCurrentPowerConsumption()<<" e ha consumato " << (*iter)->getPowerUsed()<< std::fixed << std::setprecision(2)<<" kWh"<<"\n";}
                    else{out << "\t - Il dispositivo \'" << (*iter)->getName() << "\' ha una potenza di "<<(*iter)->getCurrentPowerConsumption()<< " e ha prodotto " << std::fixed <<(*iter)->getPowerUsed() << std::setprecision(2) <<" kWh"<<"\n";}              
                }
            } else {out<<"Comando non riconosciuto. Riprovare." <<"\n";}
            break;
        

        case firstCommand::reset:
            switch(resetToSwitch(words[1])){
                case resetCommand::timeReset:   //"reset time"
                    activeDevices.clear();                             
                    setTime(CustomTime(0,0));
                    for(int i=0; i<deviceCount; i++){deviceList[i]->reset();}

                    break;
                case resetCommand::timersReset: //"reset timers"
                    for(auto itAsync = asyncDevices.begin(); itAsync!=asyncDevices.end(); itAsync++){out<< "["<< currentTime << "] Rimosso il timer dal dispositivo \'" << (*itAsync).second.second->getName() <<"\'" <<"\n";}
                    for(int i=0; i< deviceCount; i++ ){
                        auto iterAll = findDeviceByNameAll(deviceList, deviceList[i]->getName());
                        auto iterActive = findDeviceByNameActive(activeDevices, deviceList[i]->getName());
                        if(iterActive != activeDevices.end()){
                            if ((*iterActive).first == (*iterActive).second->getEndTime()){
                                activeDevices.erase(iterActive);
                                activeDevices.insert(std::pair<CustomTime, Device*>((*iterAll)->getEndTime(), *iterAll));
                            }
                            out<< "["<< currentTime << "] Rimosso il timer dal dispositivo \'" << (*iterAll)->getName() <<"\'" <<"\n";
                        }
                    }
                    asyncDevices.clear();                                //Rimuovi timer
                    break;
                case resetCommand::allReset:    //"reset all"
                    activeDevices.clear();                          //Spegni dispositivi attivi
                    asyncDevices.clear();                              //Rimuovi timer
                    setTime(CustomTime(0,0));
                    for(int i=0; i<deviceCount; i++){deviceList[i]->reset();}
                    break;
                default:
                    out<< "[" << currentTime << "] Comando non riconosciuto. Riprovare." <<"\n";
                    break;
                }
            break;

        default:
            out<< "[" << currentTime << "] Comando non riconosciuto. Riprovare." <<"\n";       
    }
    if(currentTime == CustomTime(23,59)){
            out<< "["<< currentTime << "] L'orario attuale e' " << currentTime <<"\n";
            out<< "[" << currentTime << "] Giornata terminata. Device Manager spento." <<"\n";
            fineGiornata=true;
        }
}

void DeviceManager::setTime(CustomTime newTime) {                 //Controllo tempi di start, end , e che non ci siano conflitti
    auto asyncIt = asyncDevices.begin();
    auto activeIt = activeDevices.begin();
    while (asyncIt != asyncDevices.end() || activeIt != activeDevices.end()){
        if(asyncIt == asyncDevices.end()){      //Guardo solo gli attivi
            if(activeIt->first <= newTime) {
                currentTime = activeIt->first;
                if(currentTime == CustomTime(23,59)){return;}
                auto activeItRemove = activeIt;
                activeIt++;
                removeDevice(activeItRemove);
            } else{
                activeIt = activeDevices.end();
            }
        } else {
            if (activeIt == activeDevices.end()){ //Guardo solo gli asincroni
                if(asyncIt->first <= newTime) {
                    currentTime = asyncIt->first;
                    if(currentTime == CustomTime(23,59)){return;}
                    auto asyncItRemove = asyncIt;
                    asyncIt++;
                    moveDevice(asyncItRemove);
                } else{
                    asyncIt = asyncDevices.end();
                }
        } else {        //guardo entrambi
            if(asyncIt->first <= activeIt->first && asyncIt->first <= newTime){
                currentTime = asyncIt->first;
                if(currentTime == CustomTime(23,59)){return;}
                auto asyncItRemove = asyncIt;
                asyncIt++;
                moveDevice(asyncItRemove);
            } else {
                if(activeIt->first <= asyncIt->first && activeIt->first <= newTime){
                    currentTime = activeIt->first;
                    if(currentTime == CustomTime(23,59)){return;}
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
    currentTime = newTime;
    out<< "["<< currentTime << "] L'orario attuale e' " << currentTime <<"\n";
}

bool DeviceManager::checkPowerConsumption(Device* d) {
    double currentDeviceConsumption = d->getCurrentPowerConsumption();
    
    //usiamo il > perchè il consumo è pensato in negativo
    return (currentDeviceConsumption + powerUse + maxPower > 0);
}

double DeviceManager::checkPowerConsumptionGeneral() {
    double currentPower = 0;
    for(auto it = activeDevices.begin(); it != activeDevices.end(); ++it) {
        currentPower += it->second->getCurrentPowerConsumption();
    }
    return currentPower;
}
