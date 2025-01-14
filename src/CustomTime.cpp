//ALESSIO FAGNANI

#include "../include/CustomTime.h"
#include <iostream>

//Costruttore di default che inizializaza a 00:00
CustomTime::CustomTime() : hour_{0}, minutes_{0} {}

//Costruttore parametrizzato
CustomTime::CustomTime(unsigned int ora, unsigned int minuti) : hour_{ora}, minutes_{minuti} 
{
    CheckTime(ora, minuti);
}

//Permette di cambiare l'ora ed i minuti
void CustomTime::SetTime(unsigned int const ora, unsigned int const minuti) {
    hour_ = ora;
    minutes_ = minuti;
    CheckTime(ora, minuti);
}

//Operatore di copia
CustomTime& CustomTime::operator= (CustomTime a) {
    hour_ = a.GetHour();
    minutes_ = a.GetMinutes();

    return *this;
}

//Overloading dell'operatore per la somma tra due CustomTime
CustomTime operator+ (const CustomTime&  time1,const CustomTime&  time2){

    unsigned int finalHour = time1.GetHour() + time2.GetHour();
    unsigned int finalMinute = time1.GetMinutes() + time2.GetMinutes();

    if(finalMinute>=60){
        finalMinute = finalMinute % 60;
        finalHour++;
    }

    //caso termine giornata
    if(finalHour >=24){
        finalHour = 0;
        finalMinute = 0;
    }

    //creo ogGetto time
    CustomTime return_time(finalHour,finalMinute);

    return return_time;
}

//Overloading dell'operatore per sommare ad un CustomTime un int di minuti
CustomTime operator+ (const CustomTime&  time, int  min) {
    if(min<0) {return (time - (min * -1));}

    unsigned int finalHour = time.GetHour();
    unsigned int finalMinute = time.GetMinutes() + min;

    if(finalMinute>=60){
        int hoursToAdd = finalMinute / 60;
        finalMinute = finalMinute % 60;
        finalHour += hoursToAdd;
    }

    //caso termine giornata
    if(finalHour >=24){
        finalHour = 0;
        finalMinute = 0;
    }

    //creo ogGetto time
    CustomTime return_time(finalHour,finalMinute);

    return return_time;
}

//Overloading dell'operatore per la differenza tra due CustomTime
CustomTime operator- (const CustomTime&  finish_time,const CustomTime&   start_time) { 
    int newHour = finish_time.GetHour() - start_time.GetHour();
    int newMinute = finish_time.GetMinutes() - start_time.GetMinutes();

    if(newMinute <0) {
        int hoursToSubtract = abs(newMinute) / 60;
        newMinute = 60 - (abs(newMinute) % 60);
        newHour -= hoursToSubtract;    

        if(newHour < 0) {
            newMinute = 0;
            newHour = 0;
        }
    }

    CustomTime newTime(newHour, newMinute);
    return newTime;
}

//Overloading dell'operatore per sottrarre ad un CustomTime un int di minuti
CustomTime operator- (const CustomTime&  time , int  min) {
    if(min<0) {return(time + (min * -1));}

    int finalHour = time.GetHour();
    int finalMinute = time.GetMinutes() - min;

    if(finalMinute < 0) {
        int hoursToSubtract = abs(finalMinute) / 60;
        finalMinute = 60 - (abs(finalMinute) % 60);
        finalHour -= hoursToSubtract;    

        if(finalHour < 0) {
            finalMinute = 0;
            finalHour = 0;
        }
    }

    //creo ogGetto time
    CustomTime return_time(finalHour,finalMinute);

    return return_time;
}

//Operatore di confronto
bool operator>(const CustomTime&  a,const CustomTime&  b) {
    if(a.GetHour() > b.GetHour() ) {return true;}
    if(a.GetHour() == b.GetHour() && a.GetMinutes() > b.GetMinutes()) {return true;}
    return false;
}

//Operatore di confronto
bool operator>=(const CustomTime&  a,const CustomTime&  b) {
    if(a.GetHour() > b.GetHour() ) {return true;}
    if(a.GetHour() == b.GetHour() && a.GetMinutes() >= b.GetMinutes()) {return true;}
    return false;
}

//Operatore di confronto
bool operator<(const CustomTime&  a,const CustomTime&  b)  {
    if(a.GetHour() < b.GetHour() ) {return true;}
    if(a.GetHour() == b.GetHour() && a.GetMinutes() < b.GetMinutes()) {return true;}
    return false;
}

//Operatore di confronto
bool operator<=(const CustomTime&  a,const CustomTime&  b) {
    if(a.GetHour() < b.GetHour() ) {return true;}
    if(a.GetHour() == b.GetHour() && a.GetMinutes() <= b.GetMinutes()) {return true;}
    return false;
}

//Operatore di output stream
std::ostream& operator<<(std::ostream& os, CustomTime a)
{   
    if(a.GetHour()<10 && a.GetMinutes()<10){return os << "0" << a.GetHour()<<":0"<<a.GetMinutes();}
    else if(a.GetHour()<10){return os << "0" << a.GetHour()<<":"<<a.GetMinutes();}
    else if(a.GetMinutes()<10){return os << a.GetHour()<<":0"<<a.GetMinutes();}
    return os << a.GetHour()<<":"<<a.GetMinutes();
}

//Controlla che l'ora ed i minuti passati in input siano validi
void CustomTime::CheckTime(int badHours,int  badMinutes){
    if(badHours < 0 || badHours > 23 || badMinutes < 0 || badMinutes > 59){
        throw std::invalid_argument("");
    }
}