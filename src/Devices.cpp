//DAVIDE MEROTTO

#include "../include/Devices.h"

//Implementazione classe Devices
void Devices:: stopDevice(){ status=false};
void Devices:: startDevice(){ status=true};
String Devices:: getName(){ return name};
const int Devices:: getID(){ return id};
bool Devices:: getStatus(){ return status};