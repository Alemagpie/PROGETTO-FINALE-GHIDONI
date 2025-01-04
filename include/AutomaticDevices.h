
//SOTTOCLASSE
class AutomaticDevices : public Devices{

    //Costruttore
    public:
        AutomaticDevices(String name, const int id, double wattProduction, bool status, Time current_time, Time time_duration);

    //Data membri
    private:
        Time stop_time;
    
}