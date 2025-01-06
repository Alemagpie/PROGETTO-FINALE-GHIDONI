#include "../include/DeviceManager.h"

DeviceManager::DeviceManager(){
    currentTime.setTime(0,0);
}

void DeviceManager::addDevice(Device& d) {
    Time key = d.getEndTime();

    //aggiungi entry con (chiave key e valore d) alla multimappa dei device attivi
}

void DeviceManager::parseInput(std::string command){
    //stream per fare il parsing del comando
    std::istringstream iss(command);
    std::string word;
    std::getline(iss, word, ' ');
    //controllo della prima parola
    if(word == "set"){
        //controllo della seconda parola
        if(std::getline(iss, word, ' ')){
            
        }




     } else if (word == "rm") {
        //controllo della seconda parola
        if(std::getline(iss, word, ' ')){
            
        } else {
            std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;
        }




    } else if (word == "show") {
        //controllo della seconda parola
        if(std::getline(iss, word, ' ')){

        } else {
            std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;
        }




    } else if (word == "reset") {
        //controllo della seconda parola
        if(std::getline(iss, word, ' ')){
            if(word == "time"){
                currentTime.setTime(0,0);
                activeDevices.clear();
                //Forse bisogna fare qualcosa con gli async
            } else if (word == "timers") {
                asyncDevices.clear();
            } else if (word == "all") {
                currentTime.setTime(0,0);
                activeDevices.clear();
                asyncDevices.clear();
            } else {
                std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;
            }
            
        }

    } else {
        std::cout<<"Comando non riconosciuto. Riprovare." << std::endl;
    }    

}