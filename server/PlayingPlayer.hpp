#ifndef PlayingPlayer_hpp
#define PlayingPlayer_hpp

#include "../common/HexagonalField.hpp"
#include "../common/Coordinates.hpp"
#include "../common/Defines.hpp"
#include "Player.hpp"
#include "ManagedPlayer.hpp"
#include "Ball.hpp"
#include <cstring> //memcpy






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
    char * serializeAttributes(char * bufferPosition); //renvoie l'extrémité du buffer, où l'on peut continuer à écrire
    //5 int : attribut
    //1 int : hasQuaffle
    

private:
    int __role;
    AxialCoordinates __currentPosition;
    bool __hasQuaffle;
    void hurted();

};

#endif // PlayingPlayer_hpp
