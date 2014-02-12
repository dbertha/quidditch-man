#ifndef Player_hpp
#define Player_hpp

#include <string>

#include "Broomstick.hpp"

using namespace std;

class Player {

    string _firstName;
    string _lastName;

    int _capacities[5];
    int _trainingLeft[5];
    int _popularity;
    bool _blocked;

    int _life; //Calculé à partir de la résistance, donc pas un attribut propre.
    Broomstick _broomstick;


 public:

    Player(string& playerSaveFile);

    string getFirstName();
    string getLastName();
    void verifyName();
    bool isNameTaken();

    int getCapacity(int capacityNumber);
    int getTrainingLeft(int capacityNumber);
    void setTrainingLeft(int capacityNumber, int value);

    int getPopularity();
    void setPopularity(int popularity);

    void lockPlayer();
    void unlockPlayer();
    bool isBlocked();

    int getLife();
    Broomstick getBroomstick();

    void setFirstName(string firstName);
    void setLastName(string lastName);
    void up(int capacityNumber); //Incrémente la capacité correspondante

    void setLife(int life);
    void setBroomstick(Broomstick broomstick);
    void gainPopularity();

};

#endif // Player_hpp
