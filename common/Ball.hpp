#ifndef BALL_HPP
#define BALL_HPP

//~ Rappel, dans HexagonalField.hpp :
//~ #define GOLDENSNITCH 14
//~ #define BLUDGER1 15
//~ #define BLUDGER2 16
//~ #define QUAFFLE 17

//TODO : role inutile ?

class Ball{
public :
    int getRole(){return __role;}
    AxialCoordinates getPosition(){return __currentPosition;}
    Ball(int role, AxialCoordinates startingPosition) : __role(role), __currentPosition(startingPosition) {}
    void moveTo(AxialCoordinates destination) {__currentPosition = destination;}
    int getSpeed(){ 
        if(__role == GOLDENSNITCH) {return GOLDENSNITCH_SPEED;}
        if((__role == BLUDGER1) or (__role == BLUDGER1)) {return BLUDGER_AUTOSPEED;} 
        return 0;
    }
        //TODO : vitesse du cognard influencé quand frappé par le joueur qui frappe ou influence seulement la distance où l'envoyer ?
private :
    int __role;
    AxialCoordinates __currentPosition;
};
#endif
