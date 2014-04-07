#ifndef ManagedPlayer_hpp
#define ManagedPlayer_hpp

#include <string>
#include <vector>

#include "Player.hpp"
#include "Broomstick.hpp"

typedef int gold;

class ManagedPlayer : public Player {

    int _trainingLeft[5];
    bool _blocked;
    bool _inAuction;

    Broomstick _broomstick;

 public:

 	ManagedPlayer();

 	ManagedPlayer& operator= (const ManagedPlayer&);

 	int getTrainingLeft(int capacityNumber) const;
    void setTrainingLeft(int capacityNumber, int value);

    void lockPlayer();
    void sellPlayer();
    void unlockPlayer();
    bool isBlocked() const;
    bool isInAuction() const;

    Broomstick getBroomstick();

    void setBroomstick(Broomstick broomstick);

    void train(int capacityNumber);
    gold getEstimatedValue();
    
    //void serialize(char * stringData); //TODO : insérer les éléments de getInformations dans le buffer pour ne pas le faire dans User

    std::vector<int> getInformations();

};

#endif //ManagedPlayer_hpp
