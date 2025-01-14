//ALESSIO FAGNANI

#ifndef PROGETTOFINALE_SRC_CUSTOMTIME_H
#define PROGETTOFINALE_SRC_CUSTOMTIME_H

#include <iostream>

//CLASSE TIME
class CustomTime{

    //Costruttore e funzioni membro 
    public:
        //Costruttore di default
        CustomTime();
        //Costruttore parametrizzato
        CustomTime(unsigned int ora, unsigned int minuti);

        //Fa il return dell'ora
        inline unsigned int GetHour() const {return hour_;}
        //Fa il return dei minuti
        inline unsigned int GetMinutes() const {return minutes_;}
        //Permette di cambiare l'ora ed i minuti
        void SetTime(unsigned int ora, unsigned int minuti);
        //Resetta l'ora a 00:00
        inline void ResetTime() {hour_ = minutes_ = 0;}
        //L'operatore = deve essere membro della classe
        CustomTime& operator= (CustomTime a);

    private:
        //Controlla che l'ora impostata sia valida
        void CheckTime(int bad_hours,int bad_minutes);
    
        unsigned int hour_;
        unsigned int minutes_;
};   


//Overloading degli operatori (necessario per permettere il confronto tra chiavi all'interno della multimappa)
CustomTime operator+ (const CustomTime&  time1,const CustomTime&  time2);
CustomTime operator+ (const CustomTime&  time, int  min);
CustomTime operator- (const CustomTime&  finish_time,const CustomTime&  start_time);
CustomTime operator- (const CustomTime&  time, int  min);
inline double operator* (double value,CustomTime time) {return (value*(time.GetHour() + (time.GetMinutes()/static_cast<double>(60))));}
inline bool operator== ( CustomTime& a, CustomTime& b) { return (a.GetHour() == b.GetHour() && a.GetMinutes() == b.GetMinutes());}
inline bool operator== (CustomTime a, CustomTime b){return (a.GetHour() == b.GetHour() && a.GetMinutes() == b.GetMinutes());}
bool operator<(const CustomTime&  a ,const CustomTime&  b );
bool operator<=(const CustomTime&  a ,const CustomTime&  b );
bool operator>(const CustomTime&  a ,const CustomTime&  b );
bool operator<=(const CustomTime&  a ,const CustomTime&  b );
std::ostream& operator<<(std::ostream& os, CustomTime a);

#endif