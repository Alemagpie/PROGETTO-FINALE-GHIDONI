#include "../include/DeviceManager.h"

DeviceManager::DeviceManager(OutputManager& outPut)
    : deviceCount{0}, deviceList(), activeDevices(), asyncDevices(), powerUse{0}, deviceInsertOrder(), fineGiornata(false), out{outPut}
{
    currentTime.setTime(0,0);
    out<<"Device Manager acceso"<<"\n";
}

bool DeviceManager::getFineGiornata() { return fineGiornata; }

void DeviceManager::setFineGiornata(bool s) {
    fineGiornata = s;
}

CustomTime DeviceManager::getCurrentTime() { return currentTime; }

void DeviceManager::addDevice(Device* dev){
    if(checkPowerConsumption(dev)) {
        dev->updateStartTime(currentTime);
        activeDevices.insert(std::pair<CustomTime, Device*>(dev->getEndTime(), dev)); //aggiungi entry con (chiave end_time e valore puntatore a d) alla multimappa dei device attivi
        deviceInsertOrder.push_back(dev);
        dev->setStatus(true);
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
    d->setStatus(false);
    out << "[" << currentTime << "] Il dispositivo \'" << d->getName() << "\' si e' spento" <<"\n";
    deviceInsertOrder.erase(std::find(deviceInsertOrder.begin(), deviceInsertOrder.end(), d));

    powerUse -= d->getCurrentPowerConsumption();
    if(powerUse<(maxPower * (-1))) {
        std::string removeLast = deviceInsertOrder.back()->getName();
        removeDevice(findDeviceByNameActive(activeDevices, removeLast));
        }
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

std::multimap<CustomTime, Device*>& DeviceManager::_init_returnActive() { return activeDevices; }
std::multimap<CustomTime, std::pair<CustomTime, Device*>>& DeviceManager::_init_returnAsync() { return asyncDevices; }
std::vector<Device*>& DeviceManager::_init_returnList() { return deviceList; }
std::vector<Device*>& DeviceManager::_init_returnInserOrder() { return deviceInsertOrder; }

//--------------------------------------------------------------------------

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