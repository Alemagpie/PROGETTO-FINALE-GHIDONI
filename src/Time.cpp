#include "../include/Time.h"

Time::Time() : hour{0}, minutes{0} {}

Time::Time(unsigned int ora, unsigned int minuti) : hour{ora}, minutes{minuti} {}

unsigned int Time::getHour() {
    return hour;
}

unsigned int Time::getMinutes() {
    return minutes;
}

void Time::setTime(unsigned int const ora, unsigned int const minuti) {
    hour = ora;
    minutes = minuti;
}

void Time::resetTime() {
    hour = minutes = 0;
}

Time& Time::operator= (Time a) {
    hour = a.getHour();
    minutes = a.getMinutes();

    return *this;
}
/*
Time& Time::operator= (Time a){
    hour = a.getHour();
    minutes = a.getMinutes();

    return *this;
}
*/

Time operator+ (Time&  time1, Time&  time2){

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

Time operator+ (Time&  time, int  min) {
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
    Time return_time(finalHour,finalMinute);

    return return_time;
}

Time operator- (Time&  finish_time, Time&   start_time) { 
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

    Time newTime(newHour, newMinute);
    return newTime;
}

Time operator- (Time&  time , int  min) {
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
    Time return_time(finalHour,finalMinute);

    return return_time;
}

double operator* (double value,Time time) { 
    return (value*(time.getHour() + (time.getMinutes()/60)));
}

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

/*
int getHoursDeltaTime(Time& a, Time& b) {

}

int getMinutesDeltaTime(Time& a, Time& b) {

}*/