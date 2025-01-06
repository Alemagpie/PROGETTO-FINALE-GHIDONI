// DAVIDE MEROTTO

//SOTTOCLASSE
class AutomaticDevices : public Devices{

    //Costruttore e funzioni membro
    public:
        AutomaticDevices(String name, const int id, double wattProduction, bool status);
        void setTimer(Time& start_time, Time& stop_time);

    //Data membri
    private:
        Time stop_time;
    
}