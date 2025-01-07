//DAVIDE MEROTTO

#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <cmath>

//CLASSE TIME
class Time{

    //Costruttore e funzioni membro 
    public:
        Time();
        Time(unsigned int ora, unsigned int minuti);
        unsigned int getHour();
        unsigned int getMinutes();
        void setTime(unsigned int ora, unsigned int minuti);
        void resetTime();

        Time& operator= (Time& a);

    //Data Membri
    private:
        unsigned int hour;
        unsigned int minutes;
};   

//HELPER FUNCTION

Time operator+ (Time& time1, Time& time2);
Time operator+ (Time& time, int min);

Time operator- (Time& finish_time, Time& start_time);
Time operator- (Time& time, int min);

double operator* (double value,Time& time);

bool operator== (Time& a, Time& b);

bool operator<(Time& a, Time& b);

bool operator>(Time& a, Time& b);

std::ostream& operator<<(std::ostream& os, Time a);

int getHoursDeltaTime(Time& a, Time& b);
int getMinutesDeltaTime(Time& a, Time& b);

#endif