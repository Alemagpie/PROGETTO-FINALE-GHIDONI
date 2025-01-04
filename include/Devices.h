
//CLASSE BASE 
class Devices{

    //funzioni membro
    public: 
        stopDevice();
        startDevice()
        getName();
        getID();
        getStatus();

    //Data membri
    private:
        String name;
        const int id;
        double wattProduction;
        bool status;

}