//RICCARDO FERRAZZO

#include "../include/DeviceManager.h"

#include <iterator>
#include <utility>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>

#include "../include/ManualDevice.h"
#include "../include/AutomaticDevice.h"
#include "../include/CustomTime.h"
#include "../include/OutputManager.h"

DeviceManager::DeviceManager(OutputManager& outPut)
    : device_count_{0}, device_list_(), active_devices_(), async_devices_(), power_use_{0}, device_insert_order_(), fine_giornata_(false), out_{outPut}
{
    current_time_.SetTime(0,0);
    out_<<"Device Manager accesso"<<"\n";
}

void DeviceManager::AddDevice(Device* const dev){
    //Controllo che aggiungendo il nuovo device, non si superi la soglia di potenza
    if(CheckPowerConsumption(dev)) {
        dev->UpdateStartTime(current_time_);
        //In caso si debba aggiornare end_time_ dei dispositivi CP
        dev->UpdateEndTime(dev->GetEndTime());    
        active_devices_.insert(std::pair<CustomTime, Device*>(dev->GetEndTime(), dev));
        device_insert_order_.push_back(dev);
        dev->StartDevice();
        power_use_ += dev->GetCurrentPowerConsumption();
        out_ << "[" << current_time_ << "] Il dispositivo \'" << dev->GetName() << "\' si e' acceso" <<"\n";
    } else {
        out_<< "[" << current_time_ << "] Superata soglia di consumo. E' stato spento il dispositivo: "<<dev->GetName()<<"\n";
    }
    
}

void DeviceManager::AddDeviceToList( Device& newDev) {
    device_list_.push_back(&newDev);
    device_count_++;
}

void DeviceManager::AddDeviceAsync(Device* dev, CustomTime Start, CustomTime End){
    //Controllo che l'ora sia successiva a quella attuale
    if(Start > current_time_) {
    async_devices_.insert(std::pair<CustomTime, std::pair<CustomTime, Device*>>(Start, std::pair<CustomTime, Device*>(End, dev))); 
    } else {
        out_<< "[" << current_time_ << "] Orario di inizio non valido";
    }
}

void DeviceManager::MoveDevice(std::multimap<CustomTime, std::pair<CustomTime, Device*>>::iterator it) {
    std::pair<CustomTime, Device*> deviceToMove = it->second;
    async_devices_.erase(it);
    //Controllo che il dispositivo non sia già acceso. In tale caso, non lo inserisco
    //in active_devices
    if(!(deviceToMove.second->GetStatus())){
        deviceToMove.second->UpdateEndTime(deviceToMove.first);
        AddDevice(deviceToMove.second);
    }else{
        out_<< "[" << current_time_ << "] Il device e' gia' attivo." <<"\n";
    }
}


void DeviceManager::RemoveDevice(std::multimap<CustomTime, Device*>::iterator it) {
    Device* d = it->second;
    active_devices_.erase(it);
    //Aggiorno il consumo energetico del device appena spento 
    d->UpdatePowerUsed(current_time_);
    d->StopDevice();
    out_ << "[" << current_time_ << "] Il dispositivo \'" << d->GetName() << "\' si e' spento" <<"\n";
    device_insert_order_.erase(std::find(device_insert_order_.begin(), device_insert_order_.end(), d));

    power_use_ -= d->GetCurrentPowerConsumption();
    //Nel caso in cui a spegnersi sia un dispositivi che aumenta la potenza massima,
    //mi assicuro che la potenza dei dispositivi rimanga al di sotto di quella massima
    if(power_use_<(kMaxPower * (-1))) {
        std::string removeLast = device_insert_order_.back()->GetName();
        RemoveDevice(utility::FindDeviceByNameActive(active_devices_, removeLast));
        }
} 

void SentenceIntoWords( std::vector<std::string>& ret, std::string sentence){
    int initpos=0;
    for(int endpos=0; endpos<sentence.size(); endpos++){
        if(sentence[endpos] == ' '){
            ret.push_back(sentence.substr(initpos, endpos-initpos));
            initpos = endpos+1;
        } 
    }
    ret.push_back(sentence.substr(initpos));
}

void DeviceManager::PrintInfoAll(std::string_view rem) const
{
    out_ << rem << "{ ";
    for (const auto& [key, value] : active_devices_)
        out_ << key << *value << ", ";
    out_ << "}\n";
    out_ << "Size=" << active_devices_.size() << '\n';
}

void DeviceManager::PrintInfoAsync(std::string_view rem) const
{
    out_ << rem << "{ ";
    for (const auto& [key, value] : async_devices_)
        out_ << key << value.first << *(value.second) <<", ";
    out_ << "}\n";
    out_ << "Size=" << async_devices_.size() << '\n';
}

CustomTime StringIntoCTime(std::string orario){
    if(std::find(orario.begin(), orario.end(), ':') == orario.end()) {throw std::invalid_argument("");}
    //Trasformo da string a int con la funzione stoi
    int newHour = std::stoi(orario.substr(0, orario.find(":")));        
    int newMin = std::stoi(orario.substr(orario.find(":")+1));
    CustomTime newTime(newHour, newMin);
    return newTime;
}

//--------------------------------------------------------------------------

//Vengono utilizzati degli enum per poter utilizzare lo switch nella funzione ParseInput

enum firstCommand{
    kBaseFirst = 0,kSet, kRm, kShow, kReset
};

firstCommand firstToSwitch(std::string& command){
    if( command == "set") return firstCommand::kSet;
    if( command == "rm") return firstCommand::kRm;
    if( command == "show") return firstCommand::kShow;
    if( command == "reset") return firstCommand::kReset;
    return firstCommand::kBaseFirst;
}

enum resetCommand{
    kBaseReset=0, kTimeReset, kTimersReset, kAllReset
};

resetCommand resetToSwitch(std::string& command){
    if( command == "time") return resetCommand::kTimeReset;
    if( command == "timers") return resetCommand::kTimersReset;
    if( command == "all") return resetCommand::kAllReset;
    return resetCommand::kBaseReset;
}


void DeviceManager::ParseInput(std::string command){
    out_<< "["<< current_time_ << "] L'orario attuale e' " << current_time_ <<"\n";      
    std::vector<std::string> words;
    SentenceIntoWords(words, command);
    //Controllo della prima parola                  
    switch(firstToSwitch(words[0])){
        case firstCommand::kSet:
            //Controllo che ci sia il giusto numero di parole nel comando
            if(words.size() >= 3 && words.size() < 5){      
                try{
                    //Comando "set time ${TIME}"
                    if(words[1] == "time"){ 
                        CustomTime newTime = StringIntoCTime(words[2]);
                        if(newTime > current_time_) {SetTime(newTime);}
                        else{ out_ << "[" <<current_time_ << "] Orario non disponibile. Inserire solo orari successivi a quello attuale." <<"\n";}
                    }else{
                        //Comando "set ${DEVICE} on"
                        if(words[2] == "on"){                   
                            auto iterAll = utility::FindDeviceByNameAll(device_list_, words[1]);
                            auto iterActive = utility::FindDeviceByNameActive(active_devices_, words[1]);

                            if (iterAll != device_list_.end() && *iterAll != nullptr) {
                                //Controllo che il device sia spento
                                if (iterActive == active_devices_.end()) {
                                    (*iterAll)->UpdateStartTime(current_time_);
                                    (*iterAll)->UpdateEndTime();
                                    AddDevice(*iterAll);
                                } else {
                                    out_ << "[" << current_time_ << "] Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." <<"\n";
                                }
                            } else if (iterActive != active_devices_.end()) {
                                out_ << "[" << current_time_ << "] Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." <<"\n";
                            } else {
                                out_ << "[" << current_time_ << "] Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        //Comando "set ${DEVICE} off"
                        }else if(words[2] == "off"){            
                            auto iterAll = utility::FindDeviceByNameAll(device_list_, words[1]);
                            auto iterActive = utility::FindDeviceByNameActive(active_devices_, words[1]);
                            //Controllo che il device sia spento
                            if(iterAll != device_list_.end() && iterActive != active_devices_.end() ){  
                                RemoveDevice(iterActive);
                            }else if(iterActive == active_devices_.end()){
                                out_ << "[" << current_time_ << "] Device non attivo. Prima di spegnere il dispositivo, e' necessario attivarlo."<<"\n";
                            } else{
                                out_ << "[" << current_time_ << "]Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        //Comando "set ${DEVICE} ${START_TIME} ${END_TIME}"
                        }else{                                  
                            auto iterAll = utility::FindDeviceByNameAll(device_list_, words[1]);
                            auto iterActive = utility::FindDeviceByNameActive(active_devices_, words[1]);
                            auto iterAsync = utility::FindDeviceByNameAsync(async_devices_, words[1]);
                            //Controllo che il nome sia corretto
                            if(iterAll != device_list_.end()){
                                CustomTime timeStart = StringIntoCTime(words[2]);
                                //Controllo che l'ipotetico tempo di accensione sia successivo al tempo attuale
                                if(timeStart > current_time_) {
                                    //Controllo se è presente il quarto parametro, e in caso se è un dispositivo manuale
                                    //a richiederlo, sennò viene ignorato
                                    if(words.size()==4 && (*iterAll)->GetType() == 'M'){                                              
                                        CustomTime timeEnd = StringIntoCTime(words[3]);
                                        AddDeviceAsync(*iterAll, timeStart, timeEnd);
                                        out_<< "["<< current_time_ << "] Impostato un timer per il dispositivo \'" << (*iterAll)->GetName() <<"\' dalle "<< timeStart << " alle " << timeEnd <<"\n";
                                    }else{
                                        AddDeviceAsync(*iterAll, timeStart, (*iterAll)->GetEndTime());
                                        out_<< "["<< current_time_ << "] Impostato un timer per il dispositivo \'" << (*iterAll)->GetName() <<"\' dalle "<< timeStart  <<"\n";
                                    }
                                }
                                else{ out_ << "[" <<current_time_ << "] Orario non disponibile. Inserire solo orari successivi a quello attuale." <<"\n";}
                            }else{
                                out_ << "Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        }
                    }
                }catch(std::invalid_argument e){
                    std::cerr<< e.what() << std::endl;
                    out_<< "Orario inserito non valido. Riprovare."<<"\n";
                }
            }else{
                out_<< "Comando non riconosciuto. Riprovare." <<"\n";
            }
            break;

        //Comando "rm ${DEVICE}"    
        case firstCommand::kRm:
        if (words.size() == 2){    
                auto iterAll = utility::FindDeviceByNameAll(device_list_, words[1]);
                //Contrllo che il nome del device sia corretto
                if(iterAll == device_list_.end()) {out_<< "[" << current_time_ << "] Device non riconosciuto. Riprovare." <<"\n";}
                else { ResetTimer(iterAll);}            
            } else {out_<< "[" << current_time_ << "] Comando non riconosciuto. Riprovare." <<"\n";}
            break;


        case firstCommand::kShow:
            //Aggiorno il consumo
            for(auto it = active_devices_.begin();it != active_devices_.end(); ++it){
                    it->second->UpdatePowerUsed(current_time_);
            }
            //Comando "show"
            if(words.size() == 1){ 
                double totalPowerUsed=0;
                double totalPowerProduced=0;
                //Calcolo l'energia consumata e quella prodotta
                for(int i=0; i<device_count_; i++){
                    if(device_list_[i]->GetCurrentPowerConsumption() < 0) {totalPowerUsed += device_list_[i]->GetPowerUsed();}
                    else{totalPowerProduced += device_list_[i]->GetPowerUsed();}
                }
                out_ << "[" << current_time_ << "] Attualmente il sistema ha prodotto " << std::fixed << totalPowerProduced << std::setprecision(2) <<" kWh e ha consumato " << std::fixed << totalPowerUsed << std::setprecision(2) <<" kWh. Nello specifico: "<<"\n";
                for(int i=0; i<device_count_; i++){
                    if(device_list_[i]->GetCurrentPowerConsumption() < 0) {out_ << "\t- Il dispositivo \'" << device_list_[i]->GetName() << "\' ha consumato " << std::abs(device_list_[i]->GetPowerUsed()) << std::fixed << std::setprecision(2)<<" kWh"<<"\n";}
                    else{out_ << "\t- Il dispositivo \'" << device_list_[i]->GetName() << "\' ha prodotto " << std::fixed <<device_list_[i]->GetPowerUsed() << std::setprecision(2) <<" kWh"<<"\n";}
                    
                }
            //Comando "show ${devicename}"
            } else if (words.size() == 2){    
                auto iter = utility::FindDeviceByNameAll(device_list_, words[1]);
                if(iter == device_list_.end()) {out_<<"Device non riconosciuto. Riprovare." <<"\n";}
                else {
                    if((*iter)->GetCurrentPowerConsumption() < 0) {out_<< "[" << current_time_ << "] Il dispositivo \'" << (*iter)->GetName() << "\' ha attualmente consumato " << std::abs((*iter)->GetPowerUsed())<< std::fixed << std::setprecision(2)<<" kWh"<<"\n";}
                    else{out_ << "[" << current_time_ << "] Il dispositivo \'" << (*iter)->GetName() << "\' ha attualmente prodotto " << std::fixed <<(*iter)->GetPowerUsed() << std::setprecision(2) <<" kWh"<<"\n";}              
                }
            } else {out_<<"Comando non riconosciuto. Riprovare." <<"\n";}
            break;
        

        case firstCommand::kReset:
            switch(resetToSwitch(words[1])){
                //"reset time"
                case resetCommand::kTimeReset:   
                    ResetTime();
                    break;
                //"reset timers"
                case resetCommand::kTimersReset: 
                    for(int i=0; i<device_count_; i++){
                        auto iterAll = utility::FindDeviceByNameAll(device_list_, device_list_[i]->GetName());
                        ResetTimer(iterAll);
                    }
                    break;
                //"reset all"
                case resetCommand::kAllReset:    
                    for(int i=0; i<device_count_; i++){
                        auto iterAll = utility::FindDeviceByNameAll(device_list_, device_list_[i]->GetName());
                        ResetTimer(iterAll);
                    }
                    ResetTime();
                    break;
                default:
                    out_<< "[" << current_time_ << "] Comando non riconosciuto. Riprovare." <<"\n";
                    break;
                }
            break;

        default:
            out_<< "[" << current_time_ << "] Comando non riconosciuto. Riprovare." <<"\n";       
    }
    //Nel caso in cui l'orario sia 23:59, il programma termina
    if(current_time_ == CustomTime(23,59)){
            out_<< "["<< current_time_ << "] L'orario attuale e' " << current_time_ <<"\n";
            out_<< "[" << current_time_ << "] Giornata terminata. Device Manager spento." <<"\n";
            fine_giornata_=true;
        }
}                                                                           

void DeviceManager::SetTime(CustomTime newTime) {                
    auto asyncIt = async_devices_.begin();
    auto activeIt = active_devices_.begin();
    //Ciclo attivo fino a che c'è sempre il primo elemento di async_devices_ o 
    //active_devices potrebbe avere chiave minore di newTime
    while (asyncIt != async_devices_.end() || activeIt != active_devices_.end()){
        //Possibile chiave del primo elemento di active_devices_ minore di newTime
        if(asyncIt == async_devices_.end()){     
            if(activeIt->first <= newTime) {
                current_time_ = activeIt->first;
                if (current_time_ == CustomTime(23,59)){return;}
                RemoveDevice(activeIt);
                activeIt = active_devices_.begin();
            } else {
                activeIt = active_devices_.end();
            }
        } else {
            //Possibile chiave del primo elemento di async_devices_ minore di newTime
            if (activeIt == active_devices_.end()) {
                if (asyncIt->first <= newTime) {
                    current_time_ = asyncIt->first;
                    if(current_time_ == CustomTime(23,59)){return;}
                    MoveDevice(asyncIt);
                    activeIt = active_devices_.begin();
                    asyncIt = async_devices_.begin();
                } else {
                    asyncIt = async_devices_.end();
                }
            //Possibile chiave del primo elemento di active_devices_ o async_devices minore 
            //di newTime. Controllo quale è la minore delle due
            } else { 
                if(asyncIt->first <= activeIt->first && asyncIt->first <= newTime){
                    current_time_ = asyncIt->first;
                    if(current_time_ == CustomTime(23,59)){return;}
                    MoveDevice(asyncIt);
                    activeIt = active_devices_.begin();
                    asyncIt = async_devices_.begin();
                } else {
                    if(activeIt->first <= asyncIt->first && activeIt->first <= newTime){
                        current_time_ = activeIt->first;
                        if(current_time_ == CustomTime(23,59)){return;}
                        RemoveDevice(activeIt);
                        activeIt = active_devices_.begin();
                    } else{
                        activeIt = active_devices_.end();
                        asyncIt = async_devices_.end();
                    }
                }
            }
        }
    }
    //Aggiornamento finale di current_time_ a newTime
    current_time_ = newTime;
    if(current_time_ == CustomTime(23,59)){return;}
    out_<< "["<< current_time_ << "] L'orario attuale e' " << current_time_ <<"\n";
}

bool DeviceManager::CheckPowerConsumption(Device* const d) const {
    double currentDeviceConsumption = d->GetCurrentPowerConsumption();
    //usiamo il > perchè il consumo è pensato in negativo
    return (currentDeviceConsumption + power_use_ + kMaxPower > 0);
}

void DeviceManager::ResetTime(){
    active_devices_.clear();                             
    SetTime(CustomTime(0,0));
    for(int i=0; i<device_count_; i++){device_list_[i]->Reset();}
    power_use_ = 0;
}

void DeviceManager::ResetTimer(std::vector<Device*>::iterator iterAll){
    auto iterAsync = utility::FindDeviceByNameAsync(async_devices_, (*iterAll)->GetName());
    auto iterActive = utility::FindDeviceByNameActive(active_devices_, (*iterAll)->GetName());
    (*iterAll)->RemoveTimer();
    //Se il device ha una accensione posticipata, la cancello  
    if(iterAsync != async_devices_.end()){
        async_devices_.erase(iterAsync);              
    }                         
    out_<< "["<< current_time_ << "] Rimosso il timer dal dispositivo \'" << (*iterAll)->GetName() <<"\'" <<"\n";
    if(iterActive != active_devices_.end()){
        //Se normalmente il device avrebbe un orario di spegnimento diverso da quello che è segnato
        //nella multimappa di quelli attivi, aggiorno la chiave del device nella multimappa con
        //l'end_time_ corretto
        if (!((*iterActive).first == (*iterActive).second->GetEndTime())){
            active_devices_.erase(iterActive);
            active_devices_.insert(std::pair<CustomTime, Device*>((*iterAll)->GetEndTime(), *iterAll));
        }
    }
}                               
