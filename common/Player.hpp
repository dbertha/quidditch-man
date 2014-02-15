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

    Player& operator= (const Player&);

    string getFirstName() const ;
    string getLastName() const ;
    void verifyName();
    bool isNameTaken();
    string getRandomName(string fileName, int line);
    string getRandomFirstName(int line);
    string getRandomLastName(int line);

    int getCapacity(int capacityNumber) const;
    void setCapacity(int capacityNumber, int value);

    void setFirstName(string firstName);
    void setLastName(string lastName);
    void up(int capacityNumber); //Incrémente la capacité correspondante

};

#endif // Player_hpp
