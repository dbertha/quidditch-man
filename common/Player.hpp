#ifndef Player_hpp
#define Player_hpp

#include <string>

#include "Broomstick.hpp"

using namespace std;

class Player {

    string _firstName;
    string _lastName;

    int _capacities[5];

 protected:
    long _offset;

 public:

    Player();
    Player(string playerSaveFile);

    string getFirstName();
    string getLastName();
    void verifyName();
    bool isNameTaken();
    string getRandomName(string fileName, int line);
    string getRandomFirstName(int line);
    string getRandomLastName(int line);

    int getCapacity(int capacityNumber);

    void setFirstName(string firstName);
    void setLastName(string lastName);
    void up(int capacityNumber); //Incrémente la capacité correspondante

};

#endif // Player_hpp
