
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