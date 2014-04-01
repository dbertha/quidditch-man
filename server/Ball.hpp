#ifndef BALL_HPP
#define BALL_HPP

#include "../common/Defines.hpp"

//~ Rappel, dans Defines.hpp :
//~ #define GOLDENSNITCH 14
//~ #define BLUDGER1 15
//~ #define BLUDGER2 16
//~ #define QUAFFLE 17

//TODO : role inutile ?
#define GOLDENSNITCH_SPEED 6
#define BLUDGER_AUTOSPEED 3

class Ball{
public :
    int getRole(){return __role;}
    AxialCoordinates getPosition(){return __currentPosition;}
    Ball(int role, AxialCoordinates startingPosition) : __role(role), __currentPosition(startingPosition) {}
    void moveTo(AxialCoordinates destination) {__currentPosition = destination;}
    int getSpeed(){ 
        if(__role == GOLDENSNITCH) {return GOLDENSNITCH_SPEED;}
        if((__role == BLUDGER1) or (__role == BLUDGER2)) {return BLUDGER_AUTOSPEED;} 
        return 0;
    }
    std::vector<Move> getMovesTo(AxialCoordinates destination){return __currentPosition.getMovesTo(destination);}
    int getDistanceTo(AxialCoordinates destination){return __currentPosition.getDistanceTo(destination);}
private :
    int __role;
    AxialCoordinates __currentPosition;
};
#endif
