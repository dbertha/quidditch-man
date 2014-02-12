#ifndef ManagedPlayer_hpp
#define ManagedPlayer_hpp

#include <string>

#include "Player.hpp"
#include "Broomstick.hpp"

typedef int gold;

class ManagedPlayer : public Player {

    int _trainingLeft[5];

 public:

 	ManagedPlayer(string playerSaveFile);

    void train(int capacityNumber);
    void updateLife(); //A QUOI SERT CETTE METHODE? (Je ne me souviens plus)
    gold getEstimatedValue(); //Renvoie l'estimation du prix du joueur, calculé selon ses attributs

    void heal();

};

#endif //ManagedPlayer_hpp
