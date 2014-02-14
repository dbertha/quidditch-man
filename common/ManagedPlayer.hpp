#ifndef ManagedPlayer_hpp
#define ManagedPlayer_hpp

#include <string>

#include "Player.hpp"
#include "Broomstick.hpp"

typedef int gold;

class ManagedPlayer : public Player {

    int _trainingLeft[5];
    int _popularity;
    bool _blocked;

    int _life; //Calculé à partir de la résistance, donc pas un attribut propre.
    Broomstick _broomstick;

 public:

 	ManagedPlayer(string playerSaveFile);

 	int getTrainingLeft(int capacityNumber);
    void setTrainingLeft(int capacityNumber, int value);

    int getPopularity();
    void setPopularity(int popularity);

    void lockPlayer();
    void unlockPlayer();
    bool isBlocked();

    int getLife();
    Broomstick getBroomstick();

    void setLife(int life);
    void setBroomstick(Broomstick broomstick);

    void gainPopularity();
    void train(int capacityNumber);
    void updateLife(); //A QUOI SERT CETTE METHODE? (Je ne me souviens plus)
    gold getEstimatedValue(); //Renvoie l'estimation du prix du joueur, calculé selon ses attributs

    void heal();

};

#endif //ManagedPlayer_hpp
