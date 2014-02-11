#ifndef Player_hpp
#define Player_hpp

#include <string>

#include "Broomstick.hpp"

using namespace std;

class Player {

    string _firstName;
    string _lastName;

    int _capacities[5];

    int _life; //Calculé à partir de la résistance, donc pas un attribut propre.
    Broomstick _broomstick;

 public:

    Player(string playerSaveFile);

    string getFirstName();
    string getLastName();
    int getCapacity(int capacityNumber);

    int getLife();
    Broomstick getBroomstick();

    void setFirstName(string firstName);
    void setLastName(string lastName);
    void up(int capacityNumber); //Incrémente la capacité correspondante

    void setLife(int life);
    void setBroomstick(Broomstick broomstick);


};

#endif // Player_hpp
