//DAVIDE MEROTTO

//SOTTOCLASSE
class ManualDevices : public Devices{

    //Costruttore e funzioni membro
    public:
        ManualDevices(String name, const int id, double wattProduction, bool status);
        void setTimer(Time& start_time);

}