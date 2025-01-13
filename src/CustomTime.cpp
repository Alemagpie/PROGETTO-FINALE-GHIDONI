//ALESSIO FAGNANI

#include "../include/CustomTime.h"

#include <iostream>

CustomTime::CustomTime() : hour{0}, minutes{0} {}

CustomTime::CustomTime(unsigned int ora, unsigned int minuti) : hour{ora}, minutes{minuti} 
{
    checkTime(ora, minuti);
}

void CustomTime::setTime(unsigned int const ora, unsigned int const minuti) {
    hour = ora;
    minutes = minuti;
    checkTime(ora, minuti);
}

CustomTime& CustomTime::operator= (CustomTime a) {
    hour = a.getHour();
    minutes = a.getMinutes();

    return *this;
}

CustomTime operator+ (CustomTime&  time1, CustomTime&  time2){

    unsigned int finalHour = time1.getHour() + time2.getHour();
    unsigned int finalMinute = time1.getMinutes() + time2.getMinutes();

    if(finalMinute>=60){
        finalMinute = finalMinute % 60;
        finalHour++;
    }

    //caso termine giornata
    if(finalHour >=24){
        finalHour = 0;
        finalMinute = 0;
    }

    //creo oggetto time
    CustomTime return_time(finalHour,finalMinute);

    return return_time;
}

CustomTime operator+ (CustomTime&  time, int  min) {
    if(min<0) {return (time - (min * -1));}

    unsigned int finalHour = time.getHour();
    unsigned int finalMinute = time.getMinutes() + min;

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

    //creo oggetto time
    CustomTime return_time(finalHour,finalMinute);

    return return_time;
}

CustomTime operator- (CustomTime&  finish_time, CustomTime&   start_time) { 
    int newHour = finish_time.getHour() - start_time.getHour();
    int newMinute = finish_time.getMinutes() - start_time.getMinutes();

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

CustomTime operator- (CustomTime&  time , int  min) {
    if(min<0) {return(time + (min * -1));}

    int finalHour = time.getHour();
    int finalMinute = time.getMinutes() - min;

    if(finalMinute < 0) {
        int hoursToSubtract = abs(finalMinute) / 60;
        finalMinute = 60 - (abs(finalMinute) % 60);
        finalHour -= hoursToSubtract;    

        if(finalHour < 0) {
            finalMinute = 0;
            finalHour = 0;
        }
    }

    //creo oggetto time
    CustomTime return_time(finalHour,finalMinute);

    return return_time;
}

bool operator>(const CustomTime&  a,const CustomTime&  b) {
    if(a.getHour() > b.getHour() ) {return true;}
    if(a.getHour() == b.getHour() && a.getMinutes() > b.getMinutes()) {return true;}
    return false;
}

bool operator>=(const CustomTime&  a,const CustomTime&  b) {
    if(a.getHour() > b.getHour() ) {return true;}
    if(a.getHour() == b.getHour() && a.getMinutes() >= b.getMinutes()) {return true;}
    return false;
}

bool operator<(const CustomTime&  a,const CustomTime&  b)  {
    if(a.getHour() < b.getHour() ) {return true;}
    if(a.getHour() == b.getHour() && a.getMinutes() < b.getMinutes()) {return true;}
    return false;
}

bool operator<=(const CustomTime&  a,const CustomTime&  b) {
    if(a.getHour() < b.getHour() ) {return true;}
    if(a.getHour() == b.getHour() && a.getMinutes() <= b.getMinutes()) {return true;}
    return false;
}

std::ostream& operator<<(std::ostream& os, CustomTime a)
{   
    if(a.getHour()<10 && a.getMinutes()<10){return os << "0" << a.getHour()<<":0"<<a.getMinutes();}
    else if(a.getHour()<10){return os << "0" << a.getHour()<<":"<<a.getMinutes();}
    else if(a.getMinutes()<10){return os << a.getHour()<<":0"<<a.getMinutes();}
    return os << a.getHour()<<":"<<a.getMinutes();
}

void CustomTime::checkTime(int badHours,int  badMinutes){
    if(badHours < 0 || badHours > 23 || badMinutes < 0 || badMinutes > 59){
        throw std::invalid_argument("");
    }
}