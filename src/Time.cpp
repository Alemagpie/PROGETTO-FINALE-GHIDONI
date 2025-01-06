#include "../include/Time.h"

Time::Time() : hour{0}, minutes{0} {}

Time::Time(unsigned int ora, unsigned int minuti) : hour{ora}, minutes{minuti} {}

unsigned int Time::getHour() {
    return hour;
}

unsigned int Time::getMinutes() {
    return minutes;
}

void Time::setTime(unsigned int ora, unsigned int minuti) {
    hour = ora;
    minutes = minuti;
}

void Time::resetTime() {
    hour = minutes = 0;
}

Time operator+ (Time& time1, Time& time2){

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
    Time return_time(finalHour,finalMinute);

    return return_time;
}

Time operator- (Time& finish_time, Time& start_time) { 
    int newHour = finish_time.getHour() - start_time.getHour();
    int newMinute = finish_time.getMinutes() - start_time.getMinutes();
    if(newMinute <0) {
        newMinute += 60;
        newHour--;
    }
    Time newTime(newHour, newMinute);
    return newTime;
}

double operator* (double value,Time& time) { }

bool operator==(Time& a, Time& b) {
    return (a.getHour() == b.getHour() && a.getMinutes() == b.getMinutes());
}

bool operator>(Time& a, Time& b) {
    if(a.getHour() > b.getHour() ) {return true;}
    if(a.getHour() == b.getHour() && a.getMinutes() > b.getMinutes()) {return true;}
    return false;
}

bool operator<(Time& a, Time& b) {
    if(a.getHour() < b.getHour() ) {return true;}
    if(a.getHour() == b.getHour() && a.getMinutes() < b.getMinutes()) {return true;}
    return false;
}

std::ostream& operator<<(std::ostream& os, Time& a)
{
    return os << "[" << a.getHour()<<":"<<a.getMinutes()<<"]";
}