#include "../include/CustomTime.h"

CustomTime::CustomTime() : hour{0}, minutes{0} {}

CustomTime::CustomTime(unsigned int ora, unsigned int minuti) : hour{ora}, minutes{minuti} {}

unsigned int CustomTime::getHour() const{
    return hour;
}

unsigned int CustomTime::getMinutes() const{
    return minutes;
}

void CustomTime::setTime(unsigned int const ora, unsigned int const minuti) {
    hour = ora;
    minutes = minuti;
}

void CustomTime::resetTime() {
    hour = minutes = 0;
}

CustomTime& CustomTime::operator= (CustomTime a) {
    hour = a.getHour();
    minutes = a.getMinutes();

    return *this;
}
/*
CustomTime& CustomTime::operator= (CustomTime a){
    hour = a.getHour();
    minutes = a.getMinutes();

    return *this;
}
*/

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

double operator* (double value,CustomTime time) { 
    return (value*(time.getHour() + (time.getMinutes()/60)));
}

bool operator==(CustomTime& a, CustomTime& b) {
    return (a.getHour() == b.getHour() && a.getMinutes() == b.getMinutes());
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
    return os << "[" << a.getHour()<<":"<<a.getMinutes()<<"]";
}

/*
int getHoursDeltaTime(CustomTime& a, CustomTime& b) {

}

int getMinutesDeltaTime(CustomTime& a, CustomTime& b) {

}*/