//DAVIDE MEROTTO

//CLASSE TIME
class Time{

    //Costruttore e funzioni membro 
    public:
        Time(unsigned int ora, unsigned int minuti);
        unsigned int getHour();
        unsigned int getMinutes();

    //Data Membri
    private:
        unsigned int hour;
        unsigned int minutes;

}   

//HELPER FUNCTION

Time& operator +(Time& time1, Time& time2){

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