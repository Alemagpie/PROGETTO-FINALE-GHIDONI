
//CLASSE BASE 
class Devices{

    //funzioni membro
    public: 
        void stopDevice();
        void startDevice()
        String getName();
        const int getID();
        bool getStatus();

    //Data membri
    private:
        String name;
        const int id;
        double wattProduction;
        bool status;

}