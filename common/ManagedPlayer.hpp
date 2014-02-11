#ifndef ManagedPlayer_hpp
#define ManagedPlayer_hpp

#include <string>

#include "Player.hpp"
#include "Broomstick.hpp"


class ManagedPlayer : public Player {

    int _popularity;
    bool _blocked;

    int _trainingLeft[5];

 public:

 	ManagedPlayer(string playerSaveFile);

 	int getPopularity();
 	void setPopularity(int popularity);

 	void lockPlayer();
 	void unlockPlayer();
 	bool isBlocked();

    void train(int capacityNumber);

    void updateLife(); //A QUOI SERT CETTE METHODE? (Je ne me souviens plus)
    void gainPopularity();
    int getEstimatedValue(); //Renvoie l'estimation du prix du joueur, calculé selon ses attributs

 private:

    void heal();


};

#endif //ManagedPlayer_hpp
