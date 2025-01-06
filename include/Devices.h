//DAVIDE MEROTTO

//CLASSE BASE 
class Devices{

    //funzioni membro
    public: 
        void stopDevice();
        void startDevice();
        String getName();
        const int getID();
        bool getStatus();
        double getWattHourProduction(Time& current_time);
        virtual void resetTimer()=0;    //funzione virtuale pura -> da sovrascrivere
        

    //Data membri
    private:
        Time start_time;
        String name;
        const int id;
        double wattProduction;
        bool status;

}