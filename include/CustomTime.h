//ALESSIO FAGNANI

#ifndef PROGETTOFINALE_SRC_CUSTOMTIME_H
#define PROGETTOFINALE_SRC_CUSTOMTIME_H

#include <iostream>

//CLASSE TIME
class CustomTime{

    //Costruttore e funzioni membro 
    public:
        CustomTime();
        CustomTime(unsigned int ora, unsigned int minuti);

        inline unsigned int getHour() const {return hour;}
        inline unsigned int getMinutes() const {return minutes;}
        void setTime(unsigned int ora, unsigned int minuti);
        inline void resetTime() {hour = minutes = 0;}
        CustomTime& operator= (CustomTime a);

    private:
        void checkTime(int badHours,int badMinutes);
    
        unsigned int hour;
        unsigned int minutes;
        
};   

//HELPER FUNCTION

CustomTime operator+ (CustomTime&  time1, CustomTime&  time2);
CustomTime operator+ (CustomTime&  time, int  min);
CustomTime operator- (CustomTime&  finish_time, CustomTime&  start_time);
CustomTime operator- (CustomTime&  time, int  min);
inline double operator* (double value,CustomTime time) {return (value*(time.getHour() + (time.getMinutes()/static_cast<double>(60))));}
inline bool operator== (CustomTime& a, CustomTime& b) { return (a.getHour() == b.getHour() && a.getMinutes() == b.getMinutes());}
inline bool operator== (CustomTime a, CustomTime b){return (a.getHour() == b.getHour() && a.getMinutes() == b.getMinutes());}
bool operator<(const CustomTime&  a ,const CustomTime&  b );
bool operator<=(const CustomTime&  a ,const CustomTime&  b );
bool operator>(const CustomTime&  a ,const CustomTime&  b );
bool operator<=(const CustomTime&  a ,const CustomTime&  b );
std::ostream& operator<<(std::ostream& os, CustomTime a);

#endif