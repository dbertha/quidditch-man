#ifndef PlayingPlayer_hpp
#define PlayingPlayer_hpp

#include "HexagonalField.hpp"
#include "Coordinates.hpp"
//#include "Match.hpp"
#include "Player.hpp"
#include "ManagedPlayer.hpp"
#include "Ball.hpp"
//#include "Position.hpp"


#define ROLE_KEEPER 0
#define ROLE_SEEKER 1
#define ROLE_CHASER 2
#define ROLE_BEATER 3


class PlayingPlayer : public Player {

public:

 	PlayingPlayer(ManagedPlayer& player, int role, AxialCoordinates startingPosition); 
    //les possibilités des différentes actions spécialisées sont gérées au niveau client, le serveur recevra juste s'il doit
    //bouger une balle ou un joueur

    int getMaxDistance();
    void handleBludger();
    bool interceptQuaffle(int distance);
    bool catchGoldenSnitch(int distance);
    void moveTo(AxialCoordinates destination); //setter
    void loseQuaffle();
    void carryQuaffle();
    bool hasQuaffle(){return __hasQuaffle;}
    AxialCoordinates getPosition() {return __currentPosition;}
    int getRole(){return __role;}

private:
    int __role;
    AxialCoordinates __currentPosition;
    bool __hasQuaffle;
    void hurted();

};

#endif // PlayingPlayer_hpp