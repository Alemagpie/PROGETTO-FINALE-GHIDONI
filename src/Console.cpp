#include "../include/Console.h"

Console::Console(DeviceManager& manager) : devManager{manager} {
    activeMultimap = devManager._init_returnActive();
    asyncMultimap = devManager._init_returnAsync();
    ListOfDevices = devManager._init_returnList();
    InsertOrderVector = devManager._init_returnInserOrder();
    DeviceCount = ListOfDevices.size();
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


void Console::parseInput(std::string command){
    devManager.out<< "["<< devManager.getCurrentTime() << "] L'orario attuale e' " << devManager.getCurrentTime() <<"\n";      
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
                        if(newTime > devManager.getCurrentTime()) {devManager.setTime(newTime);}
                        else{ devManager.out << "[" <<devManager.getCurrentTime() << "] Orario non disponibile. Inserire solo orari successivi a quello attuale." <<"\n";}
                    }else{
                        if(words[2] == "on"){                   //set ${DEVICE} on
                            auto iterAll = findDeviceByNameAll(ListOfDevices, words[1]);
                            auto iterActive = findDeviceByNameActive(activeMultimap, words[1]);

                            if (iterAll != ListOfDevices.end() && *iterAll != nullptr) {
                                if (iterActive == activeMultimap.end()) {
                                    (*iterAll)->updateStartTime(devManager.getCurrentTime());
                                    (*iterAll)->updateEndTime();
                                    devManager.addDevice(*iterAll);
                                } else {
                                    devManager.out << "[" << devManager.getCurrentTime() << "] Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." <<"\n";
                                }
                            } else if (iterActive != activeMultimap.end()) {
                                devManager.out << "[" << devManager.getCurrentTime() << "] Device già attivo. Se si vuole modificare i suoi orari, spegnerlo e riprovare." <<"\n";
                            } else {
                                devManager.out << "[" << devManager.getCurrentTime() << "] Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        }else if(words[2] == "off"){            //set ${DEVICE} off
                            auto iterAll = findDeviceByNameAll(ListOfDevices, words[1]);
                            auto iterActive = findDeviceByNameActive(activeMultimap, words[1]);
                            if(iterAll != ListOfDevices.end() && iterActive != activeMultimap.end() ){  
                                devManager.removeDevice(iterActive);
                                //print_infoAll("Multimappa attivi: ");
                            }else if(iterActive == activeMultimap.end()){
                                devManager.out << "[" << devManager.getCurrentTime() << "] Device non attivo. Prima di spegnere il dispositivo, e' necessario attivarlo."<<"\n";
                            } else{
                                devManager.out << "[" << devManager.getCurrentTime() << "]Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        }else{                                  //set ${DEVICE} ${START_TIME} ${END_TIME}
                            auto iterAll = findDeviceByNameAll(ListOfDevices, words[1]);
                            auto iterActive = findDeviceByNameActive(activeMultimap, words[1]);
                            auto iterAsync = findDeviceByNameAsync(asyncMultimap, words[1]);
                            if(iterAll != ListOfDevices.end()){
                                if(std::find(words[2].begin(), words[2].end(), ':') == words[2].end()) {throw std::invalid_argument("");}
                                int startHour = std::stoi(words[2].substr(0, words[2].find(":")));        //Trasformo da string a int con la funzione stoi
                                int startMin = std::stoi(words[2].substr(words[2].find(":")+1));
                                CustomTime timeStart(startHour,startMin);
                                if(timeStart > devManager.getCurrentTime()) {
                                    if(words.size()==4 && (*iterAll)->getType() == 'M'){                                              //Se c'è il parametro END_TIME
                                        int endHour = std::stoi(words[3].substr(0, words[3].find(":")));
                                        int endMin = std::stoi(words[3].substr(words[3].find(":")+1));
                                        CustomTime timeEnd(endHour, endMin);
                                        devManager.addDeviceAsync(*iterAll, timeStart, timeEnd);
                                        devManager.out<< "["<< devManager.getCurrentTime() << "] Impostato un timer per il dispositivo \'" << (*iterAll)->getName() <<"\' dalle "<< timeStart << " alle " << timeEnd <<"\n";
                                    }else{
                                        devManager.addDeviceAsync(*iterAll, timeStart, (*iterAll)->getEndTime());
                                        devManager.out<< "["<< devManager.getCurrentTime() << "] Impostato un timer per il dispositivo \'" << (*iterAll)->getName() <<"\' dalle "<< timeStart  <<"\n";
                                    }
                                }
                                else{ devManager.out << "[" <<devManager.getCurrentTime() << "] Orario non disponibile. Inserire solo orari successivi a quello attuale." <<"\n";}
                            }else{
                                devManager.out << "Device non riconosciuto. Fare attenzione al nome riportato." <<"\n";
                            }
                        }
                    }
                }catch(std::invalid_argument e){
                    devManager.out<< "Orario inserito non valido. Riprovare."<<"\n";
                }
            }else{
                devManager.out<< "Comando non riconosciuto. Riprovare." <<"\n";
            }
            break;


        case firstCommand::rm:
        if (words.size() == 2){    //"rm ${DEVICE}"      Rimuovere i timer associati ad un dispositivo.
                auto iterAll = findDeviceByNameAll(ListOfDevices, words[1]);
                if(iterAll == ListOfDevices.end()) {devManager.out<< "[" << devManager.getCurrentTime() << "] Comando non riconosciuto. Riprovare." <<"\n";}
                else {
                    auto iterAsync = findDeviceByNameAsync(asyncMultimap, words[1]);
                    auto iterActive = findDeviceByNameActive(activeMultimap, words[1]);
                    (*iterAll)->removeTimer();  
                    if(iterAsync != asyncMultimap.end()){
                        asyncMultimap.erase(iterAsync);              //DA SISTEMARE FORSE
                    }                         
                    devManager.out<< "["<< devManager.getCurrentTime() << "] Rimosso il timer dal dispositivo \'" << (*iterAll)->getName() <<"\'" <<"\n";
                    if(iterActive != activeMultimap.end()){
                        if ((*iterActive).first == (*iterActive).second->getEndTime()){
                            activeMultimap.erase(iterActive);
                            activeMultimap.insert(std::pair<CustomTime, Device*>((*iterAll)->getEndTime(), *iterAll));
                        }
                    }
                }              
            } else {devManager.out<< "[" << devManager.getCurrentTime() << "] Comando non riconosciuto. Riprovare." <<"\n";}
            break;


        case firstCommand::show:
            //aggiorno il consumo
            for(auto it = activeMultimap.begin();it != activeMultimap.end(); ++it){
                    it->second->updatePowerUsed(devManager.getCurrentTime());
            }
            if(words.size() == 1){  //"show "
                double totalPowerUsed=0;
                double totalPowerDevices = devManager.checkPowerConsumptionGeneral();
                for(int i=0; i<DeviceCount; i++){
                    if(ListOfDevices[i]->getPowerUsed() < 0) {totalPowerUsed += ListOfDevices[i]->getPowerUsed();}
                }
                devManager.out << "[" << devManager.getCurrentTime() << "] Attualmente il sistema ha consumato " << std::fixed << totalPowerUsed << std::setprecision(2) <<"kWh e la potenza massima accumulata fra i dispositivi accesi è: "<<totalPowerDevices<<". Nello specifico:"<<"\n";
                for(int i=0; i<DeviceCount; i++){
                    if(ListOfDevices[i]->getCurrentPowerConsumption() < 0) {devManager.out << "\t - Il dispositivo \'" << ListOfDevices[i]->getName() << "\' ha una potenza di "<<ListOfDevices[i]->getCurrentPowerConsumption()<<" e ha consumato " << ListOfDevices[i]->getPowerUsed() << std::fixed << std::setprecision(2)<<" kWh"<<"\n";}
                    else{devManager.out << "\t - Il dispositivo \'" << ListOfDevices[i]->getName() << "\' ha una potenza di "<<ListOfDevices[i]->getCurrentPowerConsumption()<< " e ha prodotto " << std::fixed <<ListOfDevices[i]->getPowerUsed() << std::setprecision(2) <<" kWh"<<"\n";}
                    
                }
            } else if (words.size() == 2){    //"show ${devicename}"
                auto iter = findDeviceByNameAll(ListOfDevices, words[1]);
                if(iter == ListOfDevices.end()) {devManager.out<<"Device non riconosciuto. Riprovare." <<"\n";}
                else {
                    if((*iter)->getCurrentPowerConsumption() < 0) {devManager.out << "\t - Il dispositivo \'" << (*iter)->getName() << "\'ha una potenza di "<<(*iter)->getCurrentPowerConsumption()<<" e ha consumato " << (*iter)->getPowerUsed()<< std::fixed << std::setprecision(2)<<" kWh"<<"\n";}
                    else{devManager.out << "\t - Il dispositivo \'" << (*iter)->getName() << "\' ha una potenza di "<<(*iter)->getCurrentPowerConsumption()<< " e ha prodotto " << std::fixed <<(*iter)->getPowerUsed() << std::setprecision(2) <<" kWh"<<"\n";}              
                }
            } else {devManager.out<<"Comando non riconosciuto. Riprovare." <<"\n";}
            break;
        

        case firstCommand::reset:
            switch(resetToSwitch(words[1])){
                case resetCommand::timeReset:   //"reset time"
                    activeMultimap.clear();                             
                    devManager.setTime(CustomTime(0,0));
                    for(int i=0; i<DeviceCount; i++){ListOfDevices[i]->reset();}

                    break;
                case resetCommand::timersReset: //"reset timers"
                    for(auto itAsync = asyncMultimap.begin(); itAsync!=asyncMultimap.end(); itAsync++){devManager.out<< "["<< devManager.getCurrentTime() << "] Rimosso il timer dal dispositivo \'" << (*itAsync).second.second->getName() <<"\'" <<"\n";}
                    for(int i=0; i< DeviceCount; i++ ){
                        auto iterAll = findDeviceByNameAll(ListOfDevices, ListOfDevices[i]->getName());
                        auto iterActive = findDeviceByNameActive(activeMultimap, ListOfDevices[i]->getName());
                        if(iterActive != activeMultimap.end()){
                            if ((*iterActive).first == (*iterActive).second->getEndTime()){
                                activeMultimap.erase(iterActive);
                                activeMultimap.insert(std::pair<CustomTime, Device*>((*iterAll)->getEndTime(), *iterAll));
                            }
                            devManager.out<< "["<< devManager.getCurrentTime() << "] Rimosso il timer dal dispositivo \'" << (*iterAll)->getName() <<"\'" <<"\n";
                        }
                    }
                    asyncMultimap.clear();                                //Rimuovi timer
                    break;
                case resetCommand::allReset:    //"reset all"
                    activeMultimap.clear();                          //Spegni dispositivi attivi
                    asyncMultimap.clear();                              //Rimuovi timer
                    devManager.setTime(CustomTime(0,0));
                    for(int i=0; i<DeviceCount; i++){ListOfDevices[i]->reset();}
                    break;
                default:
                    devManager.out<< "[" << devManager.getCurrentTime() << "] Comando non riconosciuto. Riprovare." <<"\n";
                    break;
                }
            break;

        default:
            devManager.out<< "[" << devManager.getCurrentTime() << "] Comando non riconosciuto. Riprovare." <<"\n";       
    }
    if(devManager.getCurrentTime() == CustomTime(23,59)){
            devManager.out<< "["<< devManager.getCurrentTime() << "] L'orario attuale e' " << devManager.getCurrentTime() <<"\n";
            devManager.out<< "[" << devManager.getCurrentTime() << "] Giornata terminata. Device Manager spento." <<"\n";
            devManager.setFineGiornata(true);
        }
}