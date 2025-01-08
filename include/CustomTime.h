//DAVIDE MEROTTO

#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <cmath>

//CLASSE TIME
class CustomTime{

    //Costruttore e funzioni membro 
    public:
        CustomTime();
        CustomTime(unsigned int ora, unsigned int minuti);
        unsigned int getHour();
        unsigned int getMinutes();
        void setTime(unsigned int ora, unsigned int minuti);
        void resetTime();

        CustomTime& operator= (CustomTime a);
        //Idk, mi da errori compilando
        //Time& operator= (Time a);

    //Data Membri
    private:
        unsigned int hour;
        unsigned int minutes;
};   

//HELPER FUNCTION

CustomTime operator+ (CustomTime&  time1, CustomTime&  time2);
CustomTime operator+ (CustomTime&  time, int  min);

CustomTime operator- (CustomTime&  finish_time, CustomTime&  start_time);
CustomTime operator- (CustomTime&  time, int  min);

double operator* (double value,CustomTime time);

bool operator== (CustomTime& a, CustomTime& b);

bool operator<(CustomTime& a, CustomTime& b);

bool operator>(CustomTime& a, CustomTime& b);

std::ostream& operator<<(std::ostream& os, CustomTime a);

#endif