#ifndef ManagedPlayer_hpp
#define ManagedPlayer_hpp

#include <string>

#include "Player.hpp"
#include "Broomstick.hpp"

typedef int gold;

class ManagedPlayer : public Player {

    int _trainingLeft[5];
    bool _blocked;

    Broomstick _broomstick;

 public:

 	ManagedPlayer();
 	ManagedPlayer(string playerSaveFile);

 	ManagedPlayer& operator= (const ManagedPlayer&);

 	int getTrainingLeft(int capacityNumber) const;
    void setTrainingLeft(int capacityNumber, int value);

    void lockPlayer();
    void unlockPlayer();
    bool isBlocked() const;

    Broomstick getBroomstick();

    void setBroomstick(Broomstick broomstick);

    void train(int capacityNumber);
    gold getEstimatedValue();

    void displayInformations();

};

#endif //ManagedPlayer_hpp
